#include "main.h"

buttonPress btnPress;
pageControlBtn pageBtn;

void IRAM_ATTR nextPageISR() {
    unsigned long current = millis();
    
    if (current - btnPress.lastNextPress < DEBOUNCE_DELAY) {
        return; //ignore the signal, as it may be noise from the button
    }
    
    btnPress.lastNextPress = current;
    pageBtn.nextPageFlag = true;
    Serial.println("Next Page");
}

void IRAM_ATTR prevPageISR() {
    unsigned long current = millis();

    if (current - btnPress.lastPrevPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastPrevPress = current;
    pageBtn.prevPageFlag = true;
    Serial.println("Prev Page");
}

void IRAM_ATTR upISR() {
    unsigned long current = millis();

    if (current - btnPress.lastUpPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastUpPress = current;
    pageBtn.upFlag = true;
    Serial.println("Up pressed!");
}

void IRAM_ATTR downISR() {
    unsigned long current = millis();

    if (current - btnPress.lastDownPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastDownPress = current;
    pageBtn.downFlag = true;
    Serial.println("Down pressed!");
}

void IRAM_ATTR selectISR() {
    unsigned long current = millis();

    if (current - btnPress.lastSelectPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastSelectPress = current;
    pageBtn.selectFlag = true;
    Serial.println("Select pressed!");
}

void IRAM_ATTR backISR() {
    unsigned long current = millis();

    if (current - btnPress.lastBackPress < DEBOUNCE_DELAY) {
        return;
    }

    btnPress.lastBackPress = current;
    pageBtn.backFlag = true;
    Serial.println("Back pressed!");
}

bool isPrevDataEmpty() {
    return isnan(prevData.prevBMETemp)
        || isnan(prevData.prevBMEHumi)
        || isnan(prevData.prevBMEPres)
        || isnan(prevData.prevBMEAlt)
        || isnan(prevData.prevSHTTemp)
        || isnan(prevData.prevSHTHumi);
}

void readAllSens() {
    readBME();
    readSHT();
}

void readHumi() {
    bme.takeForcedMeasurement();

    prevData.prevBMEHumi = bme.readHumidity();
    sensors_event_t humi, _;

    sht40.getEvent(&humi, &_);
    prevData.prevSHTHumi = humi.relative_humidity;
}

void readTemp() {
    bme.takeForcedMeasurement();

    prevData.prevBMETemp = bme.readTemperature();
    sensors_event_t temp, _;

    sht40.getEvent(&_, &temp);
    prevData.prevSHTTemp = temp.temperature;
}

void readPres() {
    bme.takeForcedMeasurement();

    prevData.prevBMEPres = bme.readPressure() / 100.0F;
}

void readAlt() {
    bme.takeForcedMeasurement();

    prevData.prevBMEAlt = bme.readAltitude(SEALEVELPRESSURE);
}

void checkFlags() {
    if (pageBtn.nextPageFlag) {
        Serial.println("next button pressed");
        
        if (currentMode == deviceMode::Options) {
            pageBtn.upFlag = true;
            pageBtn.nextPageFlag = false;
            return;
        
        }
        nextPage();
        pageBtn.nextPageFlag = false;
    }
    if (pageBtn.prevPageFlag) {
        Serial.println("prev button pressed");

        if (currentMode == deviceMode::Options) {
            pageBtn.downFlag = true;
            pageBtn.prevPageFlag = false;
            return;
        }

        prevPage();
        pageBtn.prevPageFlag = false;
    }
    if (pageBtn.upFlag) {
        switch (currentMode) {
            case deviceMode::General:
                Serial.println("Up button pressed in general mode");
                prevPage();
                break;
            case deviceMode::Settings:
                Serial.println("Up button pressed in settings mode");
                upSettings();
                break;
            case deviceMode::Options:
                Serial.println("Up button pressed in options mode");
                changeOptionValue(true); //increment option value
                break;
        }
        pageBtn.upFlag = false;
    }
    if (pageBtn.downFlag) {
        switch (currentMode) {
            case deviceMode::General:
                Serial.println("Down button pressed in general mode");
                nextPage();
                break;
            case deviceMode::Settings:
                Serial.println("Down button pressed in settings mode");
                downSettings();
                break;
            case deviceMode::Options:
                Serial.println("Down button pressed in options mode");
                changeOptionValue(false); //decrement option value
                break;
        }
        pageBtn.downFlag = false;
    }
    if (pageBtn.selectFlag) {
        Serial.println("Select button pressed");
        switch (currentMode) {
            case deviceMode::General:
                Serial.println("Select button pressed in general mode");
                break;
            case deviceMode::Settings:
                Serial.println("Select button pressed in settings mode");
                enterOptions();
                currentMode = deviceMode::Options;
                break;
            case deviceMode::Options:
                Serial.println("Select button pressed in options mode");
                pageBtn.backFlag = true; //go back to settings page
                break;
        }
        pageBtn.selectFlag = false;
    }
    if (pageBtn.backFlag) {
        Serial.println("Back button pressed");
        switch (currentMode) {
            case deviceMode::General:
                Serial.println("Back button pressed in general mode");
                break;
            case deviceMode::Settings:
                Serial.println("Back button pressed in settings mode");
                goToFirstPage();
                break;
            case deviceMode::Options:
                Serial.println("Back button pressed in options mode");
                currentMode = deviceMode::Settings;
                display.clearDisplay();
                printSettingsPageStats();
                printSettingsPageData();
                highlightOption();
                break;
        }
        pageBtn.backFlag = false;
    }
}

bool isFlagChanged() {
    unsigned long current = millis();

    if (current - btnPress.lastFlagChanged < DEBOUNCE_DELAY) {
        return false;
    }

    btnPress.lastFlagChanged = current;
    return pageBtn.nextPageFlag || pageBtn.prevPageFlag || pageBtn.upFlag || pageBtn.downFlag || pageBtn.selectFlag || pageBtn.backFlag;
}

void enterSleepMode() {
    sleepMode = true;

    display.ssd1306_command(SSD1306_DISPLAYOFF); //sets display off to save power
}

void exitSleepMode() {
    sleepMode = false;

    display.ssd1306_command(SSD1306_DISPLAYON);
}