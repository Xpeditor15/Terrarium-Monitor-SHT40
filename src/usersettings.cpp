#include "main.h"


static const settingStructures SETTINGS[] = {
    {
        SettingID::DataRefreshDelay,
        SettingType::Number,
        "Refresh Delay",
        "Refresh",
        "ms",
        &userSettings::dataRefreshDelay,
        2000, //min delay is 2 seconds, max delay is 1 minute, with 0.5 seconds steps
        60000,
        500,
        nullptr, //not a bool
        nullptr //not an enum
    },
    {
        SettingID::SleepDelay,
        SettingType::Number,
        "Sleep Delay",
        "Sleep",
        "ms",
        &userSettings::sleepDelay,
        5000, //min delay is 5 seconds, max delay is 1 minute, with 1 second steps
        60000, 
        1000,
        nullptr,
        nullptr
    },
    {
        SettingID::AlwaysOn,
        SettingType::Bool,
        "Always On",
        "Always On",
        "",
        nullptr,
        0, 
        0, 
        0, 
        &userSettings::alwaysOn,
        nullptr //not an enum
    },
    {
        SettingID::HeatingOn,
        SettingType::Bool,
        "Heating",
        "Heating",
        "",
        nullptr,
        0,
        0,
        0,
        &userSettings::heatingOn,
        nullptr
    },
    {
        SettingID::HeatingDelay,
        SettingType::Number,
        "Heating Delay",
        "Heat Delay",
        "ms",
        &userSettings::heatingDelay,
        2000, //min delay 2 seconds, max delay 10 minutes, with 1 second intervals
        600000,
        1000,
        nullptr,
        nullptr
    },
    {
        SettingID::HeatingDuration,
        SettingType::Number,
        "Heat Duration",
        "Duration",
        "ms",
        &userSettings::heatingDuration,
        100, 
        2000, 
        100,
        nullptr, 
        nullptr
    }
};


userSettings settings; //initialize user settings
int highlightedOption = static_cast<int>(SettingID::DataRefreshDelay);


settingStructures settingsDisplayed[4]; //declare an array to hold the currently displayed settings options, can only hold 4 due to screen size

void printDefaultSettingsData() {
    int currentLine = 24; //records the top most y position of the cursor
    size_t settingsCount = static_cast<size_t>(SettingID::Count); //calculates the number of setting options
    int index = 0;

    settingsDisplayed[0] = SETTINGS[highlightedOption];
    Serial.printf("Highlighted option index: 0, %s\n", settingsDisplayed[0].name);
    settingsDisplayed[1] = SETTINGS[(highlightedOption + 1) % settingsCount];
    settingsDisplayed[2] = SETTINGS[(highlightedOption + 2) % settingsCount];
    settingsDisplayed[3] = SETTINGS[(highlightedOption + 3) % settingsCount]; //update the currently displayed settings options according to what is currently highlighted

    for (int i = 0; i < 4; i++) {
        
        if (1) {
            Serial.println("Name is null");
            display.print("->");
            display.setCursor(18, currentLine);
            Serial.printf("Line %d: %s\n", currentLine, SETTINGS[(highlightedOption+i) % settingsCount].name);
            display.println(SETTINGS[(highlightedOption+i) % settingsCount].name);
            currentLine += 8;
        } 

        display.display();
    }
    highlightOption();
    display.display(); //display all setting options onto the screen
}

void printSettingsPageData() { //prints 4 settings options onto the screen, with the currently highlighted option always at the top
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 24);
    printDefaultSettingsData();
}

void clearSettingsPageData() {
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(1);
    display.setCursor(0, 24);
    printDefaultSettingsData();
}


void highlightOption() { //highlights the currently selected settings option
    display.fillRect(0, 24, 128, 8, SSD1306_WHITE); //create the highlight around the first option
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(0, 24);
    display.print("-> ");
    display.println(SETTINGS[highlightedOption].name);
    display.display();
}


void upSettings() { //moves the highlight up by one option
    Serial.println("Up button pressed in settings mode");
    clearSettingsPageData();
    uint8_t count = static_cast<uint8_t>(SettingID::Count); //gets the total number of options available
    highlightedOption = (highlightedOption - 1 + count) % count;
    printSettingsPageData();
    highlightOption();
}

void downSettings() {
    Serial.println("Down button pressed in settings mode");
    clearSettingsPageData();
    uint8_t count = static_cast<uint8_t>(SettingID::Count); //gets the total number of options available
    highlightedOption = (highlightedOption + 1) % count; //adds count to avoid negative modulo
    printSettingsPageData();
    highlightOption();
}


void enterOptions() { //choose the currently highlighted option to change
    display.clearDisplay();
    display.display();
    printSettingsPageStats(); //prints the setting page title
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    
    settingStructures selectedOption = SETTINGS[highlightedOption]; //get the selected setting structure

    display.setCursor(0, 24);
    display.printf("%s", selectedOption.shortName);

    switch (selectedOption.settingsType) {
        case SettingType::Number: {
            unsigned long currentValue = settings.*(selectedOption.numberField);
            display.setTextSize(1);
            display.setCursor(0, 40);
            display.printf("%lu %s", currentValue, selectedOption.unit);
            display.display();
            break;
        }
        case SettingType::Bool: {
            char boolStr[4];
            if (settings.*(selectedOption.boolField)) strcpy(boolStr, "On");
            else strcpy(boolStr, "Off");
            display.setTextSize(1);
            display.setCursor(0, 40);
            display.printf("%s", boolStr);
            display.display();
            break;
        }
    }
}


void changeOptionValue(settingStructures settingOption, bool condition) { //condition is used to specify if user is incrementing or decrementing. only applicable for number types
    switch (settingOption.settingsType) {
        case SettingType::Number: {
            unsigned long &value = settings.*(settingOption.numberField);
            if (condition) { //true is used to signify incrementing
                value += settingOption.step;
            } else if (!condition) {
                value -= settingOption.step;
            }

            if (value > settingOption.maxVal) { //reset the value to min value if exceeding max
                value = settingOption.minVal;
            } else if (value < settingOption.minVal) {
                value = settingOption.maxVal;
            }

            Serial.printf("Changed %s to %lu %s\n", settingOption.name, value, settingOption.unit);
            break;
        }
        case SettingType::Bool: {
            bool &value = settings.*(settingOption.boolField);
            value = !value; //toggle the boolean value
            Serial.printf("Changed %s to %s\n", settingOption.name, value ? "true" : "false");
            break;
        }
    }
}