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
    display.println("Humi: ");
    display.drawBitmap(60, 0, imageData, 16, 16, 1);
    display.setCursor(0, 24);
    display.println("BME: ");
    display.setCursor(0, 48);
    display.println("SHT: ");
    display.display();
}

void printHumiPageData() {
    
}