#include <Arduino.h>
#include <Math.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SHT4x.h>
#include "driver/rtc_io.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "esp_mac.h"


//General definitions
#define SECOND_SDA 37
#define SECOND_SCL 38

struct prevDataStruct {
    float prevBMETemp;
    float prevBMEHumi;
    float prevBMEPres;
    float prevBMEAlt;
    float prevSHTTemp;
    float prevSHTHumi;
};

extern prevDataStruct prevData;


//Definitions for 0.96 inch OLED screen
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define OLED_SDA 35
#define OLED_SCL 36

extern Adafruit_SSD1306 display;


//Definitions for BME280 sensor
#define SEALEVELPRESSURE (1013.25)
#define BMEDELAY 1000

extern Adafruit_BME280 bme;
extern Adafruit_Sensor* bmeTemp;
extern Adafruit_Sensor* bmeHumi;
extern Adafruit_Sensor* bmePres;


//Definitions for SHT40 sensor
extern Adafruit_SHT4x sht40;


//Function declarations
bool setupDisplay();
void resetSetupDisplay();
void printDefaultStats();
void test();
void resetPrintedData();
void printData();
bool setupBME(TwoWire* i2cbme, uint8_t addr = 0x76);
void printBME();
void readBME();
bool setupSHT(TwoWire* i2cSHT);
void printSHT();
void readSHT();
bool isPrevDataEmpty();