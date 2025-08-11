/*
I2C Addresses: 
- 0.96 inch OLED screen: 0x3c
- BME280: 0x76
- SHT40: 0x44
*/


#include "main.h"


TwoWire secondWire = TwoWire(1);


//Create instances of OLED display, BME280, SHT40
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;
Adafruit_SHT4x sht40 = Adafruit_SHT4x();

//Define Unified Sensor
Adafruit_Sensor *bmeTemp = bme.getTemperatureSensor();
Adafruit_Sensor *bmeHumi = bme.getHumiditySensor();
Adafruit_Sensor *bmePres = bme.getPressureSensor();


void setup() {
    Serial.begin(115200);
    Wire.begin(OLED_SDA, OLED_SCL, 400000); //Initialize both I2C buses
    secondWire.begin(SECOND_SDA, SECOND_SCL, 400000);

    if (!setupDisplay()) {
      for(;;);
    }

    if (!setupBME(&secondWire, 0x76)) {
      for(;;);
    }

    if (!setupSHT(&secondWire)) {
      for(;;);
    }

    delay(3000);
    resetSetupDisplay();
    printDefaultStats();
}

void loop() {
    printData();
    delay(3000);
}