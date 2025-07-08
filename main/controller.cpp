#include <Arduino.h>
#include "controller.h"
#include "hardware.h"

bool checkPhotoresistor(bool currentPhotoresistorIsDark, bool lastPhotoresistorIsDark) 
{
    if (currentPhotoresistorIsDark != lastPhotoresistorIsDark) {
        if (currentPhotoresistorIsDark) {
            Serial.println("Photoresistor: CHANGEMENT -> SOMBRE");
        } else {
            Serial.println("Photoresistor: CHANGEMENT -> LUMIERE");
        }
    }
    return currentPhotoresistorIsDark;
}

void checkTrashStatus(bool isTilted, unsigned long &lastBlink, bool &ledStatus, int distance)
{
    if (isTilted) {

        // Clignotement jaune si incliné (priorité la plus haute)
        if (millis() - lastBlink >= BLINK_INTERVAL_MEGA) {
        lastBlink = millis();
        ledStatus = !ledStatus;
        setColorMega(ledStatus ? 255 : 0, ledStatus ? 255 : 0, 0); // Jaune clignotant
        }
    } else {

        // Non incliné, appliquer la logique de distance
        // S'assurer que la LED n'est pas en cours de clignotement si elle vient de l'état incliné
        if (ledStatus) {
            setColorMega(0, 0, 0); // Éteindre si elle clignotait
            ledStatus = false;
        }
    
        if (distance >= 0 && distance <= 4) { // 0 à 3 cm
            setColorMega(255, 0, 0);   // Rouge: très proche
        } else if (distance > 4 && distance < 7) { // 4 à 7 cm
            setColorMega(0, 0, 255);   // Bleu: distance moyenne
        } else { // Distance >= 6cm (et non inclinée)
            setColorMega(0, 255, 0);   // Vert: loin / par défaut
        }
    }
}
