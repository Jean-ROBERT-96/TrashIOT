#ifndef CONTROLLER_H
#define CONTROLLER_H

// Gestion Wifi
#define TIMEOUT_AT_COMMAND 10000 //Timeout pour les commandes AT
#define HTTP_RESPONSE_TIMEOUT 15000 // Timeout pour la réponse HTTP après envoi
#define SEND_INTERVAL_MS 30000 // Envoyer les données toutes les 30 secondes

// Gestion Hardware
#define DISTANCE_MAX 200 // Distance maximale en cm
#define PHOTORESISTOR_DARK_THRESHOLD 140 // Seuil pour détecter l'obscurité
#define BLINK_INTERVAL_MEGA 100

/**
 * @brief Met à jour le statut du photoresistor
*/
bool checkPhotoresistor(bool, bool);

/**
 * @brief Met à jour la led selon les informations de la poubelle
*/
void checkTrashStatus(bool isTilted, unsigned long &lastBlink, bool &ledStatus, int distance);

#endif