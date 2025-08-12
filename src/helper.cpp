#include "main.h"

void IRAM_ATTR nextPageISR() {
    nextPage();
}

void IRAM_ATTR prevPageISR() {
    prevPage();
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
    sensors_event_t temp, _;

    sht40.getEvent(&_, &temp);
    prevData.prevSHTTemp = temp.temperature;
}