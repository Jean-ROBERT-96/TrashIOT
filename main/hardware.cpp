#include <Arduino.h>
#include "hardware.h"

void setColorMega(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void initiliazePinModes() {

  // Initialisation des broches des capteurs
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TILT_PIN, INPUT_PULLUP);
 
  // Initialisation des broches des LEDs 
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // LED bleue au démarrage pour indiquer que l'Arduino est prêt
  setColorMega(0, 0, 255);
  Serial.println("Arduino Mega prêt. Initialisation de l'ESP8266...");
}