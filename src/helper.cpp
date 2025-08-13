#include "main.h"

bool nextPageFlag = false;
bool prevPageFlag = false;

void IRAM_ATTR nextPageISR() {
    Serial.println("next page");
    nextPageFlag = true;
}

void IRAM_ATTR prevPageISR() {
    prevPageFlag = true;
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

void checkFlags() {
    if (nextPageFlag) {
        Serial.println("next button pressed");
        nextPage();
        nextPageFlag = false;
    }
    if (prevPageFlag) {
        Serial.println("prev button pressed");
        prevPage();
        prevPageFlag = false;
    }
}