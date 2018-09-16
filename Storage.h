//
// Created by bublik on 22-Aug-18.
//

#ifndef INTERVALOMETER_STORAGE_H
#define INTERVALOMETER_STORAGE_H

#include "ProfileTools.h"
#include "menu/ValueSelector.h"

#define MAX_PROFILE_COUNT 10 //+ zero index for current profile
#define STORAGE_FORMAT_VERSION 88

struct Settings {
    long displayTimeout = NO_TIME_VALUE;
    bool shortLabels = true;
    bool loadCurrentProfileOnStart = true;
    bool showEvSelectHelp = false;
    bool saveCurrentProfile = false; //often overwriting flash memory increases read/write speed, causes its damage, slows down program, may cause errors, speeds up global warming and deorbits satellites
};

class Storage {
public:
    static void saveProfile(int index, Profile &profile);
    static Profile getProfile(int index);
    static void saveSettings(Settings settings);
    static Settings getSettings();
    static void resetAll();
    static bool isInitialized();
private:
    static int getMemoryIndex(int profileIndex);
    static int getSettingsIndex();
    static bool isIndexInRange(int index);
    static int fixProfileIndex(int index);
};

#endif //INTERVALOMETER_STORAGE_H
