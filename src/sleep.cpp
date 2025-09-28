#include "main.h"

volatile unsigned long lastActivityTime = 0;

void checkSleepTimer(volatile unsigned long lastActivityTime) {
    if (alwaysOn) {
        Serial.println("Display is in always-on mode");
        return;
    }

    if ((millis() - lastActivityTime) >= settings.sleepDelay) { //the device has been inactive past user-defined sleepDelay
        Serial.println("Inactivity timeout reached, going into sleep mode");
        
        
    }
}