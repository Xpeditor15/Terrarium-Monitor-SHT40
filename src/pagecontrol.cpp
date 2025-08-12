#include "main.h"

bool hasPrintedStats = false;

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
    switch (currentPage) {
         case Page::Humidity:
            humidityPage();
            break;
    }
}

void humidityPage() {
    if (previousPage != Page::Count) {
        clearPrintedPage(previousPage);
    }
    
    if (!hasPrintedStats) {
        printHumiPageStats();
        hasPrintedStats = true;
        Serial.println("printedHumiStats, hasPrintedStats set to true");
    }
    printHumiPageData();
    Serial.println("printedHumiData");
}