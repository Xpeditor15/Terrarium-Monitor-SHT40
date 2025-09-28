#include <Arduino.h>
#include <Math.h>
#include <Wire.h>
#include <stdint.h>
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
#define SECOND_SDA 40
#define SECOND_SCL 41
#define NEXT_BUT 15
#define PREV_BUT 17
<<<<<<< Updated upstream
<<<<<<< Updated upstream
#define powerSwitch 42 //sends pulse every 10 seconds to PSU to keep it awake
#define USE_EXT0_WAKEUP 1 //enable sleep mode
#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)
extern unsigned long lastPing;
extern unsigned long lastActivityTime; //used to monitor last button press for entering sleep mode
=======
=======
>>>>>>> Stashed changes
#define SELECT_BUT 12
#define BACK_BUT 11

#define powerSwitch 42

extern unsigned long lastPing;
extern volatile bool alwaysOn;
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

#define DEBOUNCE_DELAY 500
#define DATA_REFRESH_DELAY 3000

//structure for page enumeration
enum class Page : uint8_t {
    Humidity,
    Temperature,
    Pressure,
    Altitude,
    Settings, 
    Count
};

//structure used to hold previous data readings
struct prevDataStruct {
    Page current;
    float prevBMETemp;
    float prevBMEHumi;
    float prevBMEPres;
    float prevBMEAlt;
    float prevSHTTemp;
    float prevSHTHumi;
};

//used to checking button debounce
struct buttonPress {
    unsigned long lastFlagChanged = 0;
    unsigned long lastNextPress = 0;
    unsigned long lastPrevPress = 0;
};

//used to track page change requests
struct pageControlBtn {
    bool nextPageFlag = false;
    bool prevPageFlag = false;
};

//used to store user specific settings
struct userSettings {
    unsigned long dataRefreshDelay = 3000;
    unsigned long sleepDelay = 60000;
};

extern Page currentPage;
extern Page previousPage;
extern prevDataStruct prevData;
extern buttonPress btnPress;
extern pageControlBtn pageBtn;
extern userSettings settings;


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

void pingPower();

//Function declarations
//Display Functions:
bool setupDisplay();
void resetSetupDisplay();
void printHumiPageStats();
void printHumiPageData();
void printTempPageStats();
void printTempPageData();
void printPresPageStats();
void printPresPageData();
void printAltPageStats();
void printAltPageData();
void clearPrintedPage(Page page);
void clearPrintedData(Page page);
void test();

//BME Functions:
bool setupBME(TwoWire* i2cbme, uint8_t addr = 0x76);
void printBME();
void readBME();

//SHT Functions:
bool setupSHT(TwoWire* i2cSHT);
void printSHT();
void readSHT();

//Helper Functions:
void IRAM_ATTR nextPageISR();
void IRAM_ATTR prevPageISR();
bool isPrevDataEmpty();
void readAllSens();
void readHumi();
void readTemp();
void readPres();
void readAlt();
void checkFlags();
bool isFlagChanged();

//Page Control Functions:
void nextPage();
void prevPage();
void goToFirstPage();
void displayPage();
void humidityPage();
void tempPage();
void presPage();
void altPage();