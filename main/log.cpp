#include <Arduino.h>
#include "log.h"
#include "hardware.h"
#include "controller.h"

bool photoresistorStatus() 
{
    return (analogRead(PHOTORESISTOR_PIN) < PHOTORESISTOR_DARK_THRESHOLD);
}

void printStatus(int distance, bool isTilted, int photoresistorValue) 
{
    Serial.print("Données brutes: Dist: ");
    Serial.print(distance);
    Serial.print("cm, Inclinaison: ");
    Serial.print(isTilted ? "Incliné" : "Droit");
    Serial.print(", Photoresistor: ");
    Serial.println(photoresistorValue);
}