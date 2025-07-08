#include <Arduino.h>
#include "NewPing.h"
#include "controller.h"
#include "hardware.h"
#include "log.h"
#include "wifi.h"

NewPing sonar(TRIGGER_PIN, ECHO_PIN, DISTANCE_MAX);
static bool lastPhotoresistorIsDark = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial1.begin(115200); // Communication avec l'ESP8266
  delay(1000);

  initiliazePinModes();

  if (!initializeESP()) 
  {
    Serial.println("ERREUR CRITIQUE: Échec de l'initialisation de l'ESP8266.");
    Serial.println("Vérifiez le câblage, l'alimentation 3.3V externe, et le firmware AT.");
    while (true);
  }
  Serial.println("Initialisation de l'ESP8266 terminée.");

  // Initialiser l'état du photoresistor pour le 1er message console
  lastPhotoresistorIsDark = photoresistorStatus();
}

void loop() {
  // put your main code here, to run repeatedly:

  static unsigned long lastSend = 0;
  static unsigned long lastBlink = 0;
  static bool ledStatus = false;

  unsigned long currentTime = millis();
  unsigned int distance = sonar.ping_cm();
  if (distance == 0) distance = DISTANCE_MAX;

  bool isTilted = (digitalRead(TILT_PIN) == HIGH);
  int photoresistorValue = analogRead(PHOTORESISTOR_PIN);
  bool currentPhotoresistorStatus = (photoresistorValue < PHOTORESISTOR_DARK_THRESHOLD);
  lastPhotoresistorIsDark = checkPhotoresistor(currentPhotoresistorStatus, lastPhotoresistorIsDark);
  
  printStatus(distance, isTilted, photoresistorValue);

  if (currentTime - lastSend >= SEND_INTERVAL_MS) 
  {    
    String payload = setPayload(distance, isTilted, photoresistorValue);
    String url = "http://" + String(TCP_HOST) + "/sensor";

    Serial.println("Envoi des données des capteurs via POST à l'API...");
    if (!sendPostRequest(url.c_str(), payload.c_str())) {
    Serial.println("AVERTISSEMENT: Échec de l'envoi des données POST. Tentative de réinitialisation de l'ESP...");
    initializeESP();
    } else {
        Serial.println("Données POST envoyées avec succès.");
    }

    lastSend = currentTime;
  } 

  checkTrashStatus(isTilted, lastBlink, ledStatus, distance);
  delay(1000);
}