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

void resetSetupDisplay() { //run once during void setup()
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(0, 0);
    display.println("Display Ready!");
    display.setCursor(0, 8);
    display.println("BME280 Ready!");
    display.setCursor(0, 16);
    display.println("SHT40 Ready!");
    display.display();
}

void printDefaultStats() { //redundant, based on old implementation
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("BME280 Temperature:");
    display.setCursor(0, 16);
    display.println("BME280 Humidity:");
    display.setCursor(0, 32);
    display.println("SHT40 Temperature: ");
    display.setCursor(0, 48);
    display.println("SHT40 Humidity");
    display.display();
}

void clearPrintedPage(Page page) { //reset the default stats for humi, temps...
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(2);

    switch (page) {
        case Page::Humidity:
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
            display.drawBitmap(60, 0, imageData, 16, 16, 1);
            display.setCursor(0, 24);
            display.println("BME: ");
            display.setCursor(60, 24);
            display.printf("%.1fC\n", prevData.prevBMEHumi);
            display.setCursor(0, 48);
            display.println("SHT: ");
            display.setCursor(60, 48);
            display.printf("%.1fC\n", prevData.prevSHTHumi);
    }

    display.display();
}

void clearPrintedData(Page page) {
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(2);

    display.setCursor(60, 24);
    display.printf("%.1fC\n", prevData.prevBMEHumi);
    display.setCursor(60, 48);
    display.printf("%.1fC\n", prevData.prevSHTHumi);

    display.display();
}

void printHumiPageStats() { //prints the default stats for humi
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

void printHumiPageData() { //prints the humidity data
    if (!isPrevDataEmpty()) {
        clearPrintedData(Page::Humidity);
    }
    readHumi();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(60, 24);
    display.printf("%.1f%\n", prevData.prevBMEHumi);
    display.setCursor(60, 48);
    display.printf("%.1f%\n", prevData.prevSHTHumi);
    display.display();
}

void resetPrintedData() { //redundant
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(0, 8);
    display.print(prevData.prevBMETemp);
    display.println("C");
    display.setCursor(0, 24);
    display.print(prevData.prevBMEHumi);
    display.println("%rh");
    display.setCursor(0, 40);
    display.print(prevData.prevSHTTemp);
    display.println("C");
    display.setCursor(0, 56);
    display.print(prevData.prevSHTHumi);
    display.println("%rh");
    display.display();
}

void printData() { //redundant
    if (isPrevDataEmpty()) {
        Serial.println("prevData empty");
    } else {
        resetPrintedData();
    }

    readAllSens();
    

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 8);
    display.print(prevData.prevBMETemp);
    display.println("C");
    display.setCursor(0, 24);
    display.print(prevData.prevBMEHumi);
    display.println("%rh");
    display.setCursor(0, 40);
    display.print(prevData.prevSHTTemp);
    display.println("C");
    display.setCursor(0, 56);
    display.print(prevData.prevSHTHumi);
    display.println("%rh");
    display.display();
}

void test() {
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
    display.clearDisplay();
    /*display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Humi: ");
    display.setCursor(0, 24);
    display.println("BME: 27.9C");
    display.setCursor(0, 48);
    display.println("SHT: 68.9%");*/
    display.drawBitmap(0, 0, imageData, 16, 16, 1);
    display.setTextSize(2);
    display.setCursor(16, 0);
    display.println("Humi: ");
    display.display();
}