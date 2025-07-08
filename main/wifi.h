#ifndef WIFI_H
#define WIFI_H

#define SSID "iPhone de Deborah"
#define PASSWORD "DebDeb24"
#define TCP_HOST "188.166.159.171"
#define PORT 80
#define API_KEY "424242"

/**
 * @brief Envoie la commande AT à l'ESP8266
 * @param command La commande AT à envoyer
 * @param expectedResponde La réponse attendue par l'arduino
 * @param timeout Le temps maximum en millisecondes pour attendre la réponse.
 * @return Vrai si la réponse attendue est reçue dans le délai, faux sinon.
*/
bool sendATCommand(char* command, char* expectedResponse, unsigned long timeout);

/**
 * @brief Initialise l'ESP8266
 * @return Vrai si toutes les étapes réussissent, faux sinon.
*/
bool initializeESP();

/**
 * @brief Tente la connexion Wifi entre le réseau spécifié et l'ESP8266
 * @return Vrai si la connexion est établie et qu'une IP est obtenue, faux sinon.
*/
bool connectWiFi();

/**
 * @brief Réinitialise l'ESP8266
*/
bool resetESP();

bool readHttpResponse(const String& expectedKeyword, unsigned long timeout, String& fullResponse);

/**
 * @brief Envoie une requête POST
 * @param url L'url de la requête
 * @param payload Contient les données à envoyer
 * @return Vrai si l'ESP a redémarré et envoyé "ready", faux sinon.
*/
bool sendPostRequest(const char* url, const char* payload);

/**
 * @brief Envoie une requête GET
 * @param url L'url de la requête
 * @return Vrai si la requête a réussi et a reçu un "200 OK", faux sinon.
*/
bool sendGetRequest(char* url);

/**
 * @brief Initialise le payload pour l'envoi
*/
String setPayload(int, bool, int);

#endif