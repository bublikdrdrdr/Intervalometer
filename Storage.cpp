//
// Created by bublik on 22-Aug-18.
//

#include <Arduino.h>
#include "Storage.h"
#include "EEPROM.h"

int Storage::getMemoryIndex(int profileIndex) {
    return sizeof(uint8_t) + sizeof(Settings) + profileIndex * sizeof(Profile);
}

int Storage::getSettingsIndex() {
    return sizeof(uint8_t);
}

int Storage::fixProfileIndex(int index) {
    if (!isIndexInRange(index)) return 0;
    return index;
}

void Storage::saveProfile(int index, Profile &profile) {
    int memoryIndex = getMemoryIndex(fixProfileIndex(index));
    EEPROM.put(memoryIndex, profile);
}

Profile Storage::getProfile(int index) {
    Profile profile = Profile();
    if (!isIndexInRange(index))
        return profile;
    return EEPROM.get(getMemoryIndex(index), profile);
}

bool Storage::isIndexInRange(int index) {
    return index >= 0 && index<=MAX_PROFILE_COUNT;
}

bool Storage::isInitialized() {
    return EEPROM.read(0)==STORAGE_FORMAT_VERSION;
}

Settings Storage::getSettings() {
    Settings settings;
    return EEPROM.get(getSettingsIndex(), settings);
}

void Storage::saveSettings(Settings settings) {
    EEPROM.put(getSettingsIndex(), settings);
}

void Storage::resetAll() {
    EEPROM.write(0, STORAGE_FORMAT_VERSION);
    Profile profile = Profile();
    for (int i = 0; i <= MAX_PROFILE_COUNT; i++){
        saveProfile(i, profile);
    }
    Settings settings = Settings();
    saveSettings(settings);
}
