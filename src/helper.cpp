#include "main.h"

buttonPress btnPress;
pageControlBtn pageBtn;

void IRAM_ATTR nextPageISR() {
    unsigned long current = millis();
    
    if (current - btnPress.lastNextPress < DEBOUNCE_DELAY) {
        return;
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
}

bool isFlagChanged() {
    unsigned long current = millis();

    if (current - btnPress.lastFlagChanged < DEBOUNCE_DELAY) {
        return false;
    }

    btnPress.lastFlagChanged = current;
    return pageBtn.nextPageFlag || pageBtn.prevPageFlag;
}