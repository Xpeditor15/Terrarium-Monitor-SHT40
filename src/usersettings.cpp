#include "main.h"

enum class SettingID : uint8_t {
    DataRefreshDelay,
    SleepDelay,
    AlwaysOn,
    HeatingOn,
    HeatingDelay,
    HeatingDuration,
    Count
};

enum class SettingType : uint8_t {
    Number,
    Bool,
    Enum,
};

struct settingStructures { //creates the structure for each individual setting options containing setting details
    SettingID id; //unique ID for each individual settings option
    SettingType settingsType; //type of the setting (Number, Enum, Bool)
    const char* name; //name (string) of the settings option to show on the display
    const char* unit; //unit (string) of the settings 

    //For number type settings
    unsigned long userSettings::*numberField; //points to the actual instance of the user settings
    unsigned long minVal;
    unsigned long maxVal;
    unsigned long step;

    //For bool type settings
    bool userSettings::*boolField;

    //For enum:
    uint8_t userSettings::*enumField;
};

static const settingStructures SETTINGS[] = {
    {
        SettingID::DataRefreshDelay,
        SettingType::Number,
        "Refresh Delay",
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
        "Heating: ",
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
        "Heating Delay: ",
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
        "Heat Duration:",
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

void printSettingsPageData() { //prints 4 settings options onto the screen, with the currently highlighted option always at the top
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 24);
    int currentLine = 24; //records the top most y position of the cursor
    size_t settingsCount = static_cast<size_t>(SettingID::Count); //calculates the number of setting options
    int index = 0;

    settingsDisplayed[0] = SETTINGS[highlightedOption];
    settingsDisplayed[1] = SETTINGS[(highlightedOption + 1) % settingsCount];
    settingsDisplayed[2] = SETTINGS[(highlightedOption + 2) % settingsCount];
    settingsDisplayed[3] = SETTINGS[(highlightedOption + 3) % settingsCount]; //update the currently displayed settings options according to what is currently highlighted

    for (int i = 0; i < 4; i++) {
        display.print("->");
        display.setCursor(18, currentLine);
        Serial.printf("Line %d: %s\n", currentLine, settingsDisplayed[i].name);
        display.println(SETTINGS[i].name);
        currentLine += 8;
    }
    display.display(); //display all setting options onto the screen
}

void highlightOption() { //highlights the currently selected settings option
    display.setTextColor(SSD1306_BLACK);
    display.fillRect(0, 24, 128, 8, SSD1306_WHITE); //create the highlight around the first option
    display.setCursor(0, 24);
    display.print("-> ");
    display.println(settingsDisplayed[0].name);
    display.display();
}

void enterOptions() { //choose the currently highlighted option to change
    display.clearDisplay();
    printSettingsPageStats(); //prints the setting page title
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    
    settingStructures selectedOption = SETTINGS[highlightedOption]; //get the selected setting structure

    display.setCursor(0, 24);
    display.printf("%s", selectedOption.name);

    switch (selectedOption.settingsType) {
        case SettingType::Number: {
            
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