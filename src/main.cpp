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

volatile bool alwaysOn = false; //false -> always-on mode off; true -> always-on mode on

void setup() {
    Serial.begin(115200);
    Wire.begin(OLED_SDA, OLED_SCL, 400000); //Initialize both I2C buses
    secondWire.begin(SECOND_SDA, SECOND_SCL, 400000);

    pinMode(NEXT_BUT, INPUT_PULLUP);
    pinMode(PREV_BUT, INPUT_PULLUP);

    pinMode(powerSwitch, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(NEXT_BUT), nextPageISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(PREV_BUT), prevPageISR, FALLING);

    //sleep mode settings
#if USE_EXT0_WAKEUP
esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(NEXT_BUT), 0);
esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(PREV_BUT), 0);
rtc_gpio_pulldown_dis(static_cast<gpio_num_t>(NEXT_BUT));
rtc_gpio_pullup_en(static_cast<gpio_num_t>(NEXT_BUT));
rtc_gpio_pulldown_dis(static_cast<gpio_num_t>(PREV_BUT));
rtc_gpio_pullup_en(static_cast<gpio_num_t>(PREV_BUT));
#endif

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
  displayPage();
  pingPower();
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