#include <Arduino.h>
#include "wifi.h"
#include "hardware.h"
#include "controller.h"

bool sendATCommand(const char* command, const char* expectedResponse, unsigned long timeout) 
{
    Serial1.println(command);

    Serial.print("Envoyé à ESP: "); 
    Serial.println(command);
    
    String response = "";
    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
        while (Serial1.available()) {
            char c = Serial1.read();
            response += c;
        }
        if (response.indexOf(expectedResponse) != -1) {
            Serial.print("Réponse reçue: "); 
            Serial.println(response);
            return true;
        }
    }
    
    Serial.print("ERREUR: Délai dépassé. Attendu '"); 
    Serial.print(expectedResponse); 
    Serial.println("'...");
    Serial.print("Réponse complète reçue: "); 
    Serial.println(response);
    return false;
}

bool resetESP()
{
    Serial.println("Réinitialisation de l'ESP8266...");
    Serial1.println("AT+RST");

    String response = "";
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // Timeout de 10 secondes
        while (Serial1.available()) {
            char c = Serial1.read();
            Serial.write(c);
            response += c;
            if (response.endsWith("ready\r\n") || response.endsWith("ready")) {
                Serial.println("\nESP8266 prêt.");
                return true;
            }
        }
    }

    Serial.println("\nERREUR: 'ready' non reçu après réinitialisation.");
    return false;
}

bool connectWiFi()
{
    Serial.println("Connexion au WiFi...");
    String cmd = "AT+CWJAP=\"" + String(SSID) + "\",\"" + String(PASSWORD) + "\"";
    return sendATCommand(cmd.c_str(), "WIFI GOT IP", 20000);
}

bool initializeESP()
{
    if (!resetESP()) return false;

    delay(500); // Petite pause après "ready"

    if (!sendATCommand("ATE0", "OK", 2000)) {
        Serial.println("AVERTISSEMENT: Impossible de désactiver l'écho (ATE0).");
    }

    if (!sendATCommand("AT+CWMODE=1", "OK", TIMEOUT_AT_COMMAND)) {
        Serial.println("ERREUR: Impossible de définir le mode WiFi sur Station.");
        return false;
    }
    if (!sendATCommand("AT+CIPMUX=0", "OK", TIMEOUT_AT_COMMAND)) {
        Serial.println("ERREUR: Impossible de définir le mode de connexion sur unique.");
        return false;
    }
    if (!connectWiFi()) {
        Serial.println("ERREUR: Échec de la connexion au WiFi.");
        return false;
    }
    return true;
}

bool readHttpResponse(const String& expectedKeyword, unsigned long timeout, String& fullResponse)
{
    fullResponse = "";
    unsigned long startTime = millis();

    while (millis() - startTime < timeout) {
        while (Serial1.available()) {
            char c = Serial1.read();
            fullResponse += c;
        }
        if (fullResponse.indexOf(expectedKeyword) != -1) {
            return true;
        }
    }

    return false;
}


bool sendPostRequest(const char* url, const char* payload)
{
    const char* host = TCP_HOST;

    String startCmd = "AT+CIPSTART=\"TCP\",\"" + String(host) + "\"," + String(PORT);
    if (!sendATCommand(startCmd.c_str(), "CONNECT", TIMEOUT_AT_COMMAND)) {
        Serial.println("ERREUR: Échec de la connexion TCP pour POST.");
        return false;
    }

    String request = "POST " + String(url) + " HTTP/1.1\r\n";
    request += "Host: " + String(host) + "\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + String(strlen(payload)) + "\r\n";
    request += "Connection: close\r\n\r\n";
    request += payload;

    String sendCmd = "AT+CIPSEND=" + String(request.length());
    if (!sendATCommand(sendCmd.c_str(), ">", TIMEOUT_AT_COMMAND)) {
        Serial.println("ERREUR: Échec de CIPSEND pour POST.");
        sendATCommand("AT+CIPCLOSE", "OK", 2000);
        return false;
    }

    // Envoi de la requête HTTP
    Serial1.print(request);
    Serial.println("Requête HTTP envoyée :");
    Serial.println(request);

    // Lecture de la réponse HTTP
    String httpResponse;
    bool ok = readHttpResponse("HTTP/1.1 200 OK", HTTP_RESPONSE_TIMEOUT, httpResponse);

    // Vérifie si réponse est correcte
    if (!ok || httpResponse.indexOf("Bad key") != -1) {
        Serial.println("Réponse HTTP reçue mais mauvaise clé API ou autre erreur :");
        Serial.println(httpResponse);
        sendATCommand("AT+CIPCLOSE", "OK", 2000);
        return false;
    }

    Serial.println("Réponse HTTP reçue avec succès :");
    Serial.println(httpResponse);

    // Vérifie si la connexion a été automatiquement fermée
    if (httpResponse.indexOf("CLOSED") == -1) {
        sendATCommand("AT+CIPCLOSE", "OK", 2000);
    }

    return true;
}


bool sendGetRequest(const char* url)
{
    const char* host = TCP_HOST;

    String startCmd = "AT+CIPSTART=\"TCP\",\"" + String(host) + "\"," + String(PORT);
    if (!sendATCommand(startCmd.c_str(), "CONNECT", TIMEOUT_AT_COMMAND)) {
        Serial.println("ERREUR: Échec de la connexion TCP pour GET Status.");
        return false;
    }

    String request = "GET " + String(url) + " HTTP/1.1\r\n";
    request += "Host: " + String(host) + "\r\n";
    request += "Connection: close\r\n\r\n";

    String sendCmd = "AT+CIPSEND=" + String(request.length());
    if (!sendATCommand(sendCmd.c_str(), ">", TIMEOUT_AT_COMMAND)) {
        Serial.println("ERREUR: Échec de CIPSEND pour GET Status.");
        sendATCommand("AT+CIPCLOSE", "OK", 2000);
        return false;
    }

    bool success = sendATCommand(request.c_str(), "HTTP/1.1 200 OK", HTTP_RESPONSE_TIMEOUT);
    sendATCommand("AT+CIPCLOSE", "OK", 2000);
    return success;
}

String setPayload(int distance, bool isTilted, int photoresistorValue)
{
    return "{\"key\":\"" + String(API_KEY) +
           "\",\"id\":" + String(ID_SENSOR) +
           ",\"distance\":" + String(distance) +
           ",\"tilt\":" + String(isTilted ? 1 : 0) +
           ",\"photo\":" + String(photoresistorValue) + "}";
}
