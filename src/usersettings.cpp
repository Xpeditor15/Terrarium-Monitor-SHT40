#include "main.h"

userSettings settings; //initialize user settings

enum class SettingID : uint8_t {
    DataRefreshDelay,
    SleepDelay,
    AlwaysOn,
    HeatingOn,
    HeatingDelay,
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
    }
};

void printSettingsPageData() {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 24);
    

}