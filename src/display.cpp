#include "main.h"

prevDataStruct prevData;

bool setupDisplay() { //run once during void setup()
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
        Serial.println(F("SSD1306 allocation failed!"));
        return false;
    }
    delay(100);
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Display Ready!");
    display.display();
    Serial.println(F("Display ready!"));
    return true;
}

void resetSetupDisplay() { //run once during void setup
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(0, 0);
    display.println("Display Ready!");
    display.setCursor(0, 8);
    display.println("BME280 Ready!");
    display.setCursor(0, 16);
    display.println("SHT40 Ready!");
    display.display();
}

void printHumiPageStats() { //prints the default words for humi page
    static const uint8_t imageData[] = {
        0x00,0x00
        ,0x00,0x00
        ,0x00,0x00
        ,0x01,0x00
        ,0x01,0x80
        ,0x01,0x80
        ,0x03,0xc0
        ,0x03,0xc0
        ,0x07,0xe0
        ,0x07,0xe0
        ,0x03,0xc0
        ,0x03,0xc0
        ,0x00,0x00
        ,0x00,0x00
        ,0x00,0x00
        ,0x00,0x00
    };
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Humi: ");
    display.drawBitmap(60, 0, imageData, 16, 16, 1);
    display.setCursor(0, 24);
    display.println("BME: ");
    display.setCursor(0, 48);
    display.println("SHT: ");
    display.display();
}

void printHumiPageData() {
    if (!isPrevDataEmpty()) {
        Serial.printf("Clearing previous data, %d\n", static_cast<uint8_t>(previousPage));
        clearPrintedData(previousPage);
    }

    readHumi();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(60, 24);
    display.printf("%.1f%%\n", prevData.prevBMEHumi);
    display.setCursor(60, 48);
    display.printf("%.1f%%\n", prevData.prevSHTHumi);
    display.display();
}

void printTempPageStats() {
    static const uint8_t imageData[] = {
        0x00,0x00,
        0x01,0x80,
        0x02,0x40,
        0x02,0x40,
        0x02,0x40,
        0x02,0x40,
        0x02,0x40,
        0x02,0x40,
        0x03,0xc0,
        0x03,0xc0,
        0x05,0xa0,
        0x07,0xe0,
        0x07,0xe0,
        0x05,0xa0,
        0x01,0xc0,
        0x00,0x00
    };
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Temp: ");
    display.drawBitmap(60, 0, imageData, 16, 16, 1);
    display.setCursor(0, 24);
    display.println("BME: ");
    display.setCursor(0, 48);
    display.println("SHT: ");
    display.display();
}

void printTempPageData() {
    if (!isPrevDataEmpty()) {
        Serial.printf("Clearing previous data, %d\n", static_cast<uint8_t>(previousPage));
        clearPrintedData(previousPage);
        if (previousPage == Page::Pressure) {
            printTempPageStats();
        }
    }

    readTemp();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(60, 24);
    display.printf("%.1fC\n", prevData.prevBMETemp);
    display.setCursor(60, 48);
    display.printf("%.1fC\n", prevData.prevSHTTemp);
    display.display();
}

void printPresPageStats() {
    static const uint8_t imageData[] ={
        0x00,0x00,
        0x07,0xc0,
        0x0c,0x60,
        0x18,0x38,
        0x70,0x0c,
        0x40,0x04,
        0xc0,0x06,
        0x40,0x04,
        0x60,0x0c,
        0x3f,0xf8,
        0x01,0xc0,
        0x0c,0x80,
        0x0c,0x20,
        0x03,0x60,
        0x01,0x00,
        0x00,0x00
    };
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Atm: ");
    display.drawBitmap(60, 0, imageData, 16, 16, 1);
    display.setCursor(0, 24);
    display.println("Pressure: ");
    display.display();
}

void printPresPageData() {
    if (!isPrevDataEmpty()) {
        Serial.printf("Clearing previous data, %d\n", static_cast<uint8_t>(previousPage));
        clearPrintedData(previousPage);
        if (previousPage == Page::Temperature) {
            printPresPageStats();
        }
    }

    readPres();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 48);
    display.printf("%.1fhPa\n", prevData.prevBMEPres);
    display.display();
}

void printAltPageStats() {
    static const uint8_t imageData[] = {
        0x00,0x00,
        0x07,0xc0,
        0x0c,0x60,
        0x18,0x38,
        0x70,0x0c,
        0x40,0x04,
        0xc0,0x06,
        0x40,0x04,
        0x60,0x0c,
        0x3f,0xf8,
        0x01,0xc0,
        0x0c,0x80,
        0x0c,0x20,
        0x03,0x60,
        0x01,0x00,
        0x00,0x00
    };
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Atm: ");
    display.drawBitmap(60, 0, imageData, 16, 16, 1);
    display.setCursor(0, 24);
    display.println("Altitude: ");
}

void printAltPageData() {
    if (!isPrevDataEmpty()) {
        Serial.printf("Clearing previous data, %d\n", static_cast<uint8_t>(previousPage));
        clearPrintedData(previousPage);
    }

    readAlt();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 48);
    display.printf("%.1fm\n", prevData.prevBMEAlt);
    display.display();
}

void printSettingsPageStats() {
    static const uint8_t imageData[] = {
        0x04,0xc0
,0x07,0xf8
,0x3f,0xf8
,0x3f,0xfc
,0x38,0x1e
,0xf0,0x0e
,0x70,0x0f
,0x70,0x0f
,0xf0,0x0e
,0xf0,0x0e
,0x70,0x0f
,0x78,0x1c
,0x7f,0xfc
,0x1f,0xfc
,0x1f,0xe0
,0x03,0x20

    };
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Settings: ");
    display.drawBitmap(112, 0, imageData, 16, 16, 1);
    display.setCursor(0, 24);
    display.display();
}

void clearPrintedData(Page page) {
    Serial.printf("In clearPrintedData, clearing data %d\n", static_cast<uint8_t>(page));
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(2);

    switch (page) {
        case Page::Humidity: {
            display.setCursor(60, 24);
            display.printf("%.1f%%\n", prevData.prevBMEHumi);
            display.setCursor(60, 48);
            display.printf("%.1f%%\n", prevData.prevSHTHumi);
            break;
        } 
        case Page::Temperature: {
            display.setCursor(60, 24);
            display.printf("%.1fC\n", prevData.prevBMETemp);
            display.setCursor(60, 48);
            display.printf("%.1fC\n", prevData.prevSHTTemp);
            break;
        }
        case Page::Pressure: {
            display.setCursor(0, 48);
            display.printf("%.1fhPa\n", prevData.prevBMEPres);
            //display.setCursor(60, 48);
            //display.printf("%.1fm\n", prevData.prevBMEAlt);
            display.display();
        }
        case Page::Altitude: {
            display.setCursor(0, 48);
            display.printf("%.1fm\n", prevData.prevBMEAlt);
            break;
        }
    }
    display.display();
}

void clearPrintedPage(Page page) {
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(2);
    Serial.printf("In clearPrintedPage, Clearing page %d\n", static_cast<uint8_t>(page));

    switch (page) {
        case Page::Humidity: {
            Serial.println("Clearing Humidity Page");
            static const uint8_t imageData[] = {
                0x00,0x00
                ,0x00,0x00
                ,0x00,0x00
                ,0x01,0x00
                ,0x01,0x80
                ,0x01,0x80
                ,0x03,0xc0
                ,0x03,0xc0
                ,0x07,0xe0
                ,0x07,0xe0
                ,0x03,0xc0
                ,0x03,0xc0
                ,0x00,0x00
                ,0x00,0x00
                ,0x00,0x00
                ,0x00,0x00
            };
            display.setCursor(0, 0);
            display.println("Humi: ");
            display.drawBitmap(60, 0, imageData, 16, 16, 0); // 0 for black to clear
            display.setCursor(0, 24);
            display.println("BME: ");
            display.setCursor(60, 24);
            display.printf("%.1f%%\n", prevData.prevBMEHumi);
            display.setCursor(0, 48);
            display.println("SHT: ");
            display.setCursor(60, 48);
            display.printf("%.1f%%\n", prevData.prevSHTHumi);
            break;
        }
        case Page::Temperature: {
            Serial.println("Clearing Temperature Page");
            static const uint8_t imageData[] = {
                0x00,0x00,
                0x01,0x80,
                0x02,0x40,
                0x02,0x40,
                0x02,0x40,
                0x02,0x40,
                0x02,0x40,
                0x02,0x40,
                0x03,0xc0,
                0x03,0xc0,
                0x05,0xa0,
                0x07,0xe0,
                0x07,0xe0,
                0x05,0xa0,
                0x01,0xc0,
                0x00,0x00
            };
            display.setCursor(0, 0);
            display.println("Temp: ");
            display.drawBitmap(60, 0,imageData, 16, 16, 0);
            display.setCursor(0, 24);
            display.println("BME: ");
            display.setCursor(60, 24);
            display.printf("%.1fC\n", prevData.prevBMETemp);
            display.setCursor(0, 48);
            display.println("SHT: ");
            display.setCursor(60, 48);
            display.printf("%.1fC\n", prevData.prevSHTTemp);
            break;
        }
        case Page::Pressure: {
            Serial.println("Clearing alt page");
            static const uint8_t imageData[] = {
                0x00,0x00,
                0x07,0xc0,
                0x0c,0x60,
                0x18,0x38,
                0x70,0x0c,
                0x40,0x04,
                0xc0,0x06,
                0x40,0x04,
                0x60,0x0c,
                0x3f,0xf8,
                0x01,0xc0,
                0x0c,0x80,
                0x0c,0x20,
                0x03,0x60,
                0x01,0x00,
                0x00,0x00
            };
            display.setCursor(0, 0);
            display.println("Atm: ");
            display.drawBitmap(60, 0, imageData, 16, 16, 0);
            display.setCursor(0, 24);
            display.println("Pressure: ");
            display.setCursor(0, 48);
            display.printf("%.1fhPa\n", prevData.prevBMEPres);
            //display.setCursor(0, 48);
            //display.println("Alt: ");
            //display.printf("%.1fm\n", prevData.prevBMEAlt);
            break;
        }
        case Page::Altitude: {
            static const uint8_t imageData[] = {
                0x00,0x00,
                0x07,0xc0,
                0x0c,0x60,
                0x18,0x38,
                0x70,0x0c,
                0x40,0x04,
                0xc0,0x06,
                0x40,0x04,
                0x60,0x0c,
                0x3f,0xf8,
                0x01,0xc0,
                0x0c,0x80,
                0x0c,0x20,
                0x03,0x60,
                0x01,0x00,
                0x00,0x00
            };
            display.setCursor(0, 0);
            display.println("Atm: ");
            display.drawBitmap(60, 0, imageData, 16, 16, 0);
            display.setCursor(0, 24);
            display.println("Altitude: ");
            display.setCursor(0, 48);
            display.printf("%.1fm\n", prevData.prevBMEAlt);
            break;
        }

    }
    display.display();
}

void test() {
    static const uint8_t imageData[] ={
0x00,0x00
,0x07,0xc0
,0x0c,0x60
,0x18,0x38
,0x70,0x0c
,0x40,0x04
,0xc0,0x06
,0x40,0x04
,0x60,0x0c
,0x3f,0xf8
,0x01,0xc0
,0x0c,0x80
,0x0c,0x20
,0x03,0x60
,0x01,0x00
,0x00,0x00

    };

    display.clearDisplay();
    display.drawBitmap(60, 0, imageData, 16, 16, 1);
    display.display();
}