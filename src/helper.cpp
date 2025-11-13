#include "main.h"

buttonPress btnPress;
pageControlBtn pageBtn;

void IRAM_ATTR nextPageISR() {
    unsigned long current = millis();
    
    if (current - btnPress.lastNextPress < DEBOUNCE_DELAY) {
        return; //ignore the signal, as it may be noise from the button
    }
    
    btnPress.lastNextPress = current;
    pageBtn.nextPageFlag = true;
    Serial.println("Next Page");
}

void IRAM_ATTR prevPageISR() {
    unsigned long current = millis();

    if (current - btnPress.lastPrevPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastPrevPress = current;
    pageBtn.prevPageFlag = true;
    Serial.println("Prev Page");
}

void IRAM_ATTR upISR() {
    unsigned long current = millis();

    Serial.println("NIGGA UP");

    if (current - btnPress.lastUpPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastUpPress = current;
    pageBtn.upFlag = true;
    Serial.println("Up pressed!");
}

void IRAM_ATTR downISR() {
    unsigned long current = millis();

    if (current - btnPress.lastDownPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastDownPress = current;
    pageBtn.downFlag = true;
    Serial.println("Down pressed!");
}

void IRAM_ATTR selectISR() {
    unsigned long current = millis();

    if (current - btnPress.lastSelectPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastSelectPress = current;
    pageBtn.selectFlag = true;
    Serial.println("Select pressed!");
}

void IRAM_ATTR backISR() {
    unsigned long current = millis();

    if (current - btnPress.lastBackPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastBackPress = current;
    pageBtn.backFlag = true;
    Serial.println("Back pressed!");
}

bool isPrevDataEmpty() {
    return isnan(prevData.prevBMETemp)
        || isnan(prevData.prevBMEHumi)
        || isnan(prevData.prevBMEPres)
        || isnan(prevData.prevBMEAlt)
        || isnan(prevData.prevSHTTemp)
        || isnan(prevData.prevSHTHumi);
}

void readAllSens() {
    readBME();
    readSHT();
}

void readHumi() {
    bme.takeForcedMeasurement();

    prevData.prevBMEHumi = bme.readHumidity();
    sensors_event_t humi, _;

    sht40.getEvent(&humi, &_);
    prevData.prevSHTHumi = humi.relative_humidity;
}

void readTemp() {
    bme.takeForcedMeasurement();

    prevData.prevBMETemp = bme.readTemperature();
    sensors_event_t temp, _;

    sht40.getEvent(&_, &temp);
    prevData.prevSHTTemp = temp.temperature;
}

void readPres() {
    bme.takeForcedMeasurement();

    prevData.prevBMEPres = bme.readPressure() / 100.0F;
}

void readAlt() {
    bme.takeForcedMeasurement();

    prevData.prevBMEAlt = bme.readAltitude(SEALEVELPRESSURE);
}

void checkFlags() {
    if (pageBtn.nextPageFlag) {
        Serial.println("next button pressed");
        nextPage();
        pageBtn.nextPageFlag = false;
    }
    if (pageBtn.prevPageFlag) {
        Serial.println("prev button pressed");
        prevPage();
        pageBtn.prevPageFlag = false;
    }
    if (pageBtn.upFlag) {
        switch (currentMode) {
            case deviceMode::General:
                Serial.println("Up button pressed in general mode");
                nextPage();
                break;
            case deviceMode::Settings:
                Serial.println("Up button pressed in settings mode");
                upSettings();
                break;
            case deviceMode::Options:
                Serial.println("Up button pressed in options mode");
                //increment option value;
                break;
        }
        pageBtn.upFlag = false;
    }
}

bool isFlagChanged() {
    unsigned long current = millis();

    if (current - btnPress.lastFlagChanged < DEBOUNCE_DELAY) {
        return false;
    }

    btnPress.lastFlagChanged = current;
    return pageBtn.nextPageFlag || pageBtn.prevPageFlag || pageBtn.upFlag || pageBtn.downFlag || pageBtn.selectFlag || pageBtn.downFlag;
}

void enterSleepMode() {
    sleepMode = true;

    display.ssd1306_command(SSD1306_DISPLAYOFF); //sets display off to save power
}

void exitSleepMode() {
    sleepMode = false;

    display.ssd1306_command(SSD1306_DISPLAYON);
}