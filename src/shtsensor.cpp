#include "main.h"

bool setupSHT(TwoWire* i2cSHT) {
    bool status = sht40.begin(i2cSHT);
    if (!status) {
        Serial.println(F("SHT40 initialization failed!"));
        return false;
    }

    Serial.println(F("SHT40 initialized!"));
    sht40.setPrecision(SHT4X_HIGH_PRECISION);
    sht40.setHeater(SHT4X_NO_HEATER);
    display.setCursor(0, 16);
    display.println("SHT40 Ready!");
    display.display();
    return true;
}


void printSHT() {
    sensors_event_t humidity, temp;

    uint32_t timestamp = millis();
    sht40.getEvent(&humidity, &temp);
    timestamp = millis() - timestamp;

    Serial.print(F("Temperature: "));
    Serial.print(temp.temperature);
    Serial.println(F(" °C"));

    Serial.print(F("Humidity: "));
    Serial.print(humidity.relative_humidity);
    Serial.println(F(" % rh"));

    Serial.print(F("Read duration (ms): "));
    Serial.println(timestamp);
}

void readSHT() {
    sensors_event_t humidity, temp;

    sht40.getEvent(&humidity, &temp);

    prevData.prevSHTTemp = temp.temperature;
    prevData.prevSHTHumi = humidity.relative_humidity;
}