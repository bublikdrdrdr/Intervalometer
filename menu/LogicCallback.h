//
// Created by bublik on 16-Aug-18.
//

#ifndef INTERVALOMETER_LOGICCALLBACK_H
#define INTERVALOMETER_LOGICCALLBACK_H

#include "../ProfileTools.h"
#include "../Display.h"
#include "../Storage.h"

struct CallbackFunctions{
    void (*setBulbOn)(bool);
    Profile &(*getCurrentProfile)();
    void (*resetCurrentProfile)();
    int (*getProfilesCount)();
    int (*getCurrentProfileId)();
    void (*setShootingPaused)(bool);
    void (*setShootingOn)(bool);
    void (*hardReset)();
    void (*setProfileId)(int);
    void (*saveCurrentProfileToId)(int);
    long (*getShootingCount)();
    Profile (*getProfileById)(int);
    void (*onCurrentProfileChange)();
    Settings *(*getSettings)();
    void (*onSettingsChange)();
};


#endif //INTERVALOMETER_LOGICCALLBACK_H
