/*
I2C Addresses: 
- 0.96 inch OLED screen: 0x3c
- BME280: 0x76
- SHT40: 0x44
*/

/*
To-Do List:
1. Need to add heating function for SHT40 when humidity reaches 100% 
  - Add fixed timer to enable heating at night automatically?
  - Let user set manually in settings to enable or disable
2. Add settings page to change data refresh rate and sleep delay
  - Disable screen but keep ESP32 on
3. Addfeature so that it checks when the up and down buttons are pressed
  - If buttons are pressed while in settings option, change the value
  - If buttons are pressed while in settings page, change the settings option
  - If buttons are pressed while in other pages, can be used as left and right button
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

//Page counters
/*
Page 0: SHT40 and BME280 Humidity
Page 1: SHT40 and BME280 Temperature
Page 2: BME280 Pressure
Page 3: BME280 Altitude
Page 3: Settings 
*/

Page currentPage = Page::Humidity;
Page previousPage = Page::Humidity; //defaults to unset/count during initialization

deviceMode currentMode = deviceMode::General; //when booting up, the device shows humidity page (in general mode)

volatile bool alwaysOn = false; //false -> always-on mode off; true -> always-on mode on
volatile bool sleepMode = false; //false -> normal mode; true -> sleep mode

void setup() {
    Serial.begin(115200);
    Wire.begin(OLED_SDA, OLED_SCL, 400000); //Initialize both I2C buses
    secondWire.begin(SECOND_SDA, SECOND_SCL, 400000);

    pinMode(NEXT_BUT, INPUT_PULLUP);
    pinMode(PREV_BUT, INPUT_PULLUP);
    pinMode(UP_BUT, INPUT_PULLUP);
    pinMode(DOWN_BUT, INPUT_PULLUP);
    pinMode(SELECT_BUT, INPUT_PULLUP);
    pinMode(BACK_BUT, INPUT_PULLUP);

    pinMode(powerSwitch, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(NEXT_BUT), nextPageISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(PREV_BUT), prevPageISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(UP_BUT), upISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(DOWN_BUT), downISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(SELECT_BUT), selectISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BACK_BUT), backISR, FALLING);

    prevData.current = currentPage;

    if (!setupDisplay()) {
      for(;;);
    }

    if (!setupSHT(&secondWire)) {
      for(;;);
    }

    if (!setupBME(&secondWire, 0x76)) {
      for(;;);
    }

    delay(3000);
    resetSetupDisplay();
}

void loop() {
  
  /*if (!sleepMode) {
    displayPage();
    pingPower();
  } else {
    Serial.println("In sleep mode, skipping displayPage");
  }*/
  
  
  printSettingsPageStats();
  printSettingsPageData();
  delay(10000);
  
}

unsigned long lastPing = 0;

void pingPower() {
  if (millis() - lastPing >= 10000) {
    lastPing = millis();
    digitalWrite(powerSwitch, HIGH);
    delay(100);
    digitalWrite(powerSwitch, LOW);
  }
}