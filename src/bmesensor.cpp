#include "main.h"

bool setupBME(TwoWire* i2cbme, uint8_t addr) {
    if (i2cbme == nullptr) {
        Serial.println(F("setupSensor: bus pointer is null"));
        return false;
    }

    if (!bme.begin(addr, i2cbme)) {
        Serial.println(F("Failed to initialize BME280"));
        return false;
    }

    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::FILTER_OFF);

    Serial.println(F("BME280 initialized!"));
    display.setCursor(0, 8);
    display.println("BME280 Ready!");
    display.display();
    return true;
}


void printBME() {
    bme.takeForcedMeasurement();
    Serial.print(F("Temperature: "));
    Serial.print(bme.readTemperature());
    Serial.println(F(" *C"));

    Serial.print(F("Pressure: "));
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(F(" hPa"));

    Serial.print(F("Approx. Altitude: "));
    Serial.print(bme.readAltitude(SEALEVELPRESSURE));
    Serial.println(F(" m"));

    Serial.print(F("Humidity: "));
    Serial.print(bme.readHumidity());
    Serial.println(F(" %"));

    Serial.println();
}

void readBME() {
    bme.takeForcedMeasurement();

    sensors_event_t temp, humi, pres;

    bmeTemp->getEvent(&temp);
    bmeHumi->getEvent(&humi);
    bmePres->getEvent(&pres);

    prevData.prevBMETemp = temp.temperature;
    prevData.prevBMEHumi = humi.relative_humidity;
    prevData.prevBMEPres = pres.pressure / 100.0F;
    prevData.prevBMEAlt = bme.readAltitude(SEALEVELPRESSURE);
}