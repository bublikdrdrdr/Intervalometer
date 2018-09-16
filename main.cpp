#include <Arduino.h>
#include "Storage.h"
#include "ShootingTimer.h"
#include "Display.h"
#include "Buttons.h"
#include "menu/LogicCallback.h"
#include "menu/ValueSelector.h"
#include "menu/MainMenu.h"

/*
 Ctrl+F9 - build and upload
 Shift+Alt+F10 - select configuration
    nano_CORE - for developing
    upload - for upload
 */

//pins
#define UP 14
#define DOWN 15
#define SET 16
#define TRG 2
#define CLK 3
#define DIO 4

Display display(CLK, DIO);
Buttons buttons(UP, DOWN, SET, true);
CallbackFunctions callbackFunctions = CallbackFunctions();
int profileId = CURRENT_VALUE;
Profile profile = Profile();
ShootingTimer shootingTimer(TRG, true);
Settings settings;
MainMenu mainMenu(callbackFunctions, display);

unsigned long lastButtonActivity = 0;

void fixStorageInitialized();

void initCallbackFunctions();

void loadSettings();

bool processDisplayTimeout(ButtonState buttonState);

void setup() {
    fixStorageInitialized();
    initCallbackFunctions();
    loadSettings();
    if (settings.saveCurrentProfile || settings.loadCurrentProfileOnStart) callbackFunctions.setProfileId(0);
    display.setFixedLength(settings.shortLabels);
    mainMenu.setTimeSelectHelp(settings.showEvSelectHelp);
    mainMenu.setCallback(callbackFunctions);
}

void loop() {
    ButtonState buttonState = buttons.getState();
    if (processDisplayTimeout(buttonState)) {
        mainMenu.onButtonClick(buttonState);
    }
    mainMenu.updateAndGetDone();
    shootingTimer.update();
    display.update();
}

bool getDisplayTimeoutState() {
    return settings.displayTimeout <= 0 || (millis() - lastButtonActivity < settings.displayTimeout);
}

bool processDisplayTimeout(ButtonState buttonState) {
    bool oldValue = getDisplayTimeoutState();
    if (buttonState.button != Button::NO_BUTTON && buttonState.state != State::NO_STATE) {
        lastButtonActivity = millis();
    }
    display.setDisplayOn(getDisplayTimeoutState());
    return oldValue;
}

void loadSettings() {
    settings = Storage::getSettings();
}

void fixStorageInitialized() {
    if (!Storage::isInitialized()) {
        Storage::resetAll();
    }
}

void fSetBulbOn(bool value) {
    shootingTimer.setState(value ? ShootingState::BULB : ShootingState::OFF);
}

Profile &fGetCurrentProfile() {
    return profile;
}

void fResetCurrentProfile() {
    profile = Profile();
}

int fGetProfilesCount() {
    return MAX_PROFILE_COUNT;
}

int fGetCurrentProfileId() {
    return profileId;
}

void fSetShootingPaused(bool value) {
    shootingTimer.setState(value ? ShootingState::PAUSED : ShootingState::ON);
    display.setBlinking(value);
}

void fSetShootingOn(bool value) {
    shootingTimer.setProfile(profile);
    shootingTimer.setState(value ? ShootingState::ON : ShootingState::OFF);
    display.setBlinking(false);
}

void (*restartArduino)() = 0;

void fHardReset() {
    settings.shortLabels = true;
    Storage::saveSettings(settings);
    loop();
    Storage::resetAll();
    restartArduino();
}

void fSetProfileId(int value) {
    profileId = value;
    profile = Storage::getProfile(value);
    shootingTimer.setProfile(profile);
}

void fSaveCurrentProfileToId(int value) {
    Storage::saveProfile(value, profile);
    profileId = value;
}

long fGetShootingCount() {
    return shootingTimer.getCount();
}

Profile fGetProfileById(int id) {
    return Storage::getProfile(id);
}

void fOnCurrentProfileChange() {
    if (settings.saveCurrentProfile) {
        Storage::saveProfile(0, profile);
    }
}

Settings *fGetSettings(){
    return &settings;
}

void fOnSettingsChange(){
    Storage::saveSettings(settings);
    display.setFixedLength(settings.shortLabels);
    mainMenu.setTimeSelectHelp(settings.showEvSelectHelp);
}

void initCallbackFunctions() {
    callbackFunctions.setBulbOn = fSetBulbOn;
    callbackFunctions.getCurrentProfile = fGetCurrentProfile;
    callbackFunctions.resetCurrentProfile = fResetCurrentProfile;
    callbackFunctions.getProfilesCount = fGetProfilesCount;
    callbackFunctions.getCurrentProfileId = fGetCurrentProfileId;
    callbackFunctions.setShootingPaused = fSetShootingPaused;
    callbackFunctions.setShootingOn = fSetShootingOn;
    callbackFunctions.hardReset = fHardReset;
    callbackFunctions.setProfileId = fSetProfileId;
    callbackFunctions.saveCurrentProfileToId = fSaveCurrentProfileToId;
    callbackFunctions.getShootingCount = fGetShootingCount;
    callbackFunctions.getProfileById = fGetProfileById;
    callbackFunctions.onCurrentProfileChange = fOnCurrentProfileChange;
    callbackFunctions.getSettings = fGetSettings;
    callbackFunctions.onSettingsChange = fOnSettingsChange;
}