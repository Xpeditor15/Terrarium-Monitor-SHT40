#include "main.h"

prevDataStruct prevData;

bool setupDisplay() {
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

void resetSetupDisplay() {
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(0, 0);
    display.println("Display Ready!");
    display.setCursor(0, 8);
    display.println("BME280 Ready!");
    display.setCursor(0, 16);
    display.println("SHT40 Ready!");
    display.display();
}

void printDefaultStats() {
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

void resetPrintedData() {
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

void printData() {
    if (isPrevDataEmpty()) {
        Serial.println("prevData empty");
    } else {
        resetPrintedData();
    }

    readBME();
    readSHT();
    

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
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("HOW BIG IS THIS");
    display.display();
}