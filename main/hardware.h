#ifndef HARDWARE_H
#define HARDWARE_H

#define ID_SENSOR 1
#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define RED_PIN 4
#define GREEN_PIN 5
#define BLUE_PIN 6
#define TILT_PIN 7

#define PHOTORESISTOR_PIN A0

/**
 * @brief Permet d'initialiser la couleur de la led
 * @param r couleur Red
 * @param g couleur Green
 * @param b couleur Blue
*/
void setColorMega(int r, int g, int b);

/**
 * @brief Initialise les différentes broches
*/
void initiliazePinModes();

#endif