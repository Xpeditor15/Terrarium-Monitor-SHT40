#include "main.h"

volatile unsigned long lastActivityTime = 0;

void checkSleepTimer(volatile unsigned long lastActivityTime) {
    if (alwaysOn) {
        Serial.println("Display is in always-on mode");
        return;
    }

    if (!sleepMode) { //if device not already in sleep mode
        if ((millis() - lastActivityTime) >= settings.sleepDelay) { //the device has been inactive past user-defined sleepDelay
            Serial.println("Inactivity timeout reached, going into sleep mode");
            
            display.ssd1306_command(SSD1306_DISPLAYOFF); //sets display off to save power
            sleepMode = true;
        }
    }
}