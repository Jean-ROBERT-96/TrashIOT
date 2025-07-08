#ifndef LOG_H
#define LOG_H

/**
 * @brief Initialise l'état du photoresistor pour le premier message console
*/
bool photoresistorStatus();

/**
 * @brief Affiche les données
 * @param int 
*/
void printStatus(int, bool, int);

#endif