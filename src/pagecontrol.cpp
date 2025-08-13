#include "main.h"

bool hasPrintedStats = false;
unsigned long prevDisplay;

void nextPage() {
    previousPage = currentPage;
    hasPrintedStats = false;
    uint8_t i = static_cast<uint8_t>(currentPage);
    uint8_t count = static_cast<uint8_t>(Page::Count);
    currentPage = static_cast<Page>((i + 1) % count);
    displayPage();
}

void prevPage() {
    previousPage = currentPage;
    hasPrintedStats = false;
    uint8_t i = static_cast<uint8_t>(currentPage);
    uint8_t count = static_cast<uint8_t>(Page::Count);
    currentPage = static_cast<Page>((i + count - 1) % count);
    displayPage();
}

void goToFirstPage() {
    currentPage = Page::Humidity;
}

void displayPage() {
    if (millis() - prevDisplay < 3000) {
        return;
    }
    prevDisplay = millis();
    checkFlags();
    switch (currentPage) {
        case Page::Humidity:{
            humidityPage();
            break;
        }
        case Page::Temperature:{
            tempPage();
            break; 
        }
    }
}

void humidityPage() {
    /*if (previousPage != Page::Count) {
        clearPrintedPage(previousPage);
    }*/
    /*if (previousPage != Page::Humidity) {
        clearPrintedPage(previousPage);
    }*/

    if (!hasPrintedStats) {
        clearPrintedPage(previousPage);
        printHumiPageStats();
        hasPrintedStats = true;
        Serial.println("printedHumiStats, hasPrintedStats set to true");
    }

    printHumiPageData();
    Serial.println("printedHumiData");
}

void tempPage() {
    /*if (previousPage != Page::Temperature) {
        clearPrintedPage(previousPage);
        Serial.printf("Cleared %d\n", static_cast<uint8_t>(previousPage));
    }*/

    if (!hasPrintedStats) {
        clearPrintedPage(previousPage);
        printTempPageStats();
        hasPrintedStats = true;
        Serial.println("printedTempStats, hasPrintedStats is true");
    }
    printTempPageData();
    Serial.println("printedTempData");
}