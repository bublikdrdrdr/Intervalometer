//
// Created by bublik on 16-Aug-18.
//

#include <Arduino.h>
#include "MainMenu.h"
#include "../Labels.h"
#include "../NumberPrinter.h"
#include "../TimeTools.h"
#include "../Storage.h"

#define mMainLength 9
#define mGoLength 4
#define mProfilesLength 4
#define mSettingsLength 7

int mMainLabels[] = {lGo, lBulb, lShutterSpeed, lDelay, lGroup, lShootsLimit, lProfiles, lReset, lSettings}; //add delay
int mGoLabels[] = {lInfo, lDone, lProfile, lPause};
int mProfilesLabels[] = {lLoad, lSave, lInfo, lDone};
int mSettingsLabels[] = {lDisplayTimeout, lFullLabels, lAutoSaveProfile, lReadOnStart, lShowSelectHelp, lHardReset,
                         lDone};

MainMenu::MainMenu(CallbackFunctions &callbackFunctions, Display &display) : callbackFunctions(callbackFunctions),
                                                                             valueSelector(display, false),
                                                                             display(display) {}

bool MainMenu::updateAndGetDone() {
    if (!valueSelector.updateAndGetDone()) {
        initialized = true;
        return true;
    }
    if (currentMenu == mBulb) {
        Label label = NumberPrinter::formatTime(millis() - bulbMillis, TimeFormat::FULL);
        label.setArrayPointer();
        display.setLabel(label);
        initialized = true;
        return true;
    } else if (currentMenu == mInfo) {
        unsigned long difference = millis() - infoMillis;
        if (difference > (infoStage == InfoStage::COUNT ? INFO_COUNT_TIME : INFO_LABELS_TIME)) {
            infoMillis = millis();
            infoStage = getNextInfoStage(infoStage, CYCLIC_INFO_MODE);
            if (infoStage == InfoStage::_END) {
                currentMenu = mGo;
            }
            initialized = false;
        }
        if (!initialized) {
            if (!updateInfoStageAndGetInitialized()) {
                currentMenu = mGo;
            } else {
                initialized = true;
            }
        }
        return true;
    } else if (currentMenu == mDetailedInfo) {
        if (millis() - infoMillis > INFO_COUNT_TIME) {
            infoMillis = millis();
            Profile profile = callbackFunctions.getCurrentProfile();
            infoStage = getNextDetailedInfoStage(infoStage, profile);
            if (infoStage == InfoStage::_END) {
                currentMenu = mProfiles;
            }
            initialized = false;
        }
        if (!initialized) {
            if (!updateDetailedInfoStageAndGetInitialized()) {
                currentMenu = mProfiles;
            } else {
                initialized = true;
            }
        }
        return true;
    }
    if (!initialized) {
        setDisplayLabelId(getCurrentLabelId());
        initialized = true;
    }
    return false;
}

void MainMenu::onButtonClick(ButtonState button) {
    if (!initialized) {
        setDisplayLabelId(getCurrentLabelId());
        initialized = true;
    }
    if (button.button == Button::NO_BUTTON || button.button == Button::MULTI || button.state == State::NO_STATE) return;
    if (updateAndGetDone()) {
        processLogic(button);
        return;
    }
    switch (button.button) {
        case Button::SET:
            if (!moveForward()) {
                processLogic(button);
                return;
            }
            break;
        case Button::UP:
            move(true);
            break;
        case Button::DOWN:
            move(false);
            break;
    }
    setDisplayLabelId(getCurrentLabelId());
}

bool MainMenu::updateInfoStageAndGetInitialized() {
    Label label;
    long count;
    switch (infoStage) {
        case InfoStage::COUNT:
            count = callbackFunctions.getShootingCount();
            label = NumberPrinter::formatTime(count * 1000L, TimeFormat::SECONDS);
            label.setArrayPointer();
            display.setLabel(label);
            break;
        case InfoStage::OF_LABEL:
            label.string = display.getLabelString(lOf);
            display.setLabel(label);
            break;
        case InfoStage::FULL_COUNT:
            count = callbackFunctions.getCurrentProfile().countLimit;
            label = NumberPrinter::formatTime(count * 1000L, TimeFormat::SECONDS);
            label.setArrayPointer();
            display.setLabel(label);
            break;
        default:
            return false;
    }
    return true;
}

void MainMenu::formatStepsTime(long value, bool stepsMode, Label &label) {
    label = stepsMode ? NumberPrinter::formatSteps(value) : NumberPrinter::formatTime(value, TimeFormat::FULL);
    label.setArrayPointer();
}

bool MainMenu::updateDetailedInfoStageAndGetInitialized() {
    Label label;
    int labelId = getStageLabelId(infoStage);
    if (labelId != -1) {
        label.string = display.getLabelString(labelId);
        display.setLabel(label);
        return true;
    }
    switch (infoStage) {
        case InfoStage::EV_MODE:
            label.string = display.getLabelString(ProfileTools::getModeLabel(detailedInfoProfile.evMode));
            break;
        case InfoStage::EV:
            formatStepsTime(detailedInfoProfile.ev, detailedInfoProfile.evStepsMode, label);
            break;
        case InfoStage::END_EV:
            formatStepsTime(detailedInfoProfile.endEv, detailedInfoProfile.evStepsMode, label);
            break;
        case InfoStage::DELAY_MODE:
            label.string = display.getLabelString(ProfileTools::getModeLabel(detailedInfoProfile.delayMode));
            break;
        case InfoStage::DELAY:
            formatStepsTime(detailedInfoProfile.delay, detailedInfoProfile.delayStepsMode, label);
            break;
        case InfoStage::END_DELAY:
            formatStepsTime(detailedInfoProfile.endDelay, detailedInfoProfile.delayStepsMode, label);
            break;
        case InfoStage::GROUP:
            label = NumberPrinter::formatTime(detailedInfoProfile.group * 1000L, TimeFormat::SECONDS);
            label.setArrayPointer();
            break;
        case InfoStage::COUNT:
            label = NumberPrinter::formatTime(detailedInfoProfile.countLimit * 1000L, TimeFormat::SECONDS);
            label.setArrayPointer();
            break;
        default:
            return false;
    }
    display.setLabel(label);
    return true;
}

int MainMenu::getStageLabelId(InfoStage infoStage) {
    switch (infoStage) {
        case InfoStage::OF_LABEL:
            return lOf;
        case InfoStage::EV_LABEL:
            return lShutterSpeed;
        case InfoStage::DELAY_LABEL:
            return lDelay;
        case InfoStage::GROUP_LABEL:
            return lGroup;
        case InfoStage::COUNT_LABEL:
            return lShootsLimit;
        default:
            return -1;
    }
}

// returns false if there is no menu route and/or some special function must be called
bool MainMenu::moveForward() {
    switch (currentMenu) {
        case mMain:
            switch (mMainLabels[currentPosition]) {
                case lProfiles:
                    currentMenu = mProfiles;
                    break;
                case lSettings:
                    currentMenu = mSettings;
                    break;
                default:
                    return false;
            }
            currentPosition = 0;
            return true;
        case mProfiles:
            if (mProfilesLabels[currentPosition] == lDone) {
                moveBack();
                return true;
            }
            return false;
        case mSettings:
            if (mSettingsLabels[currentPosition] == lDone) {
                moveBack();
                return true;
            }
            return false;
    }
    return false;

}

void MainMenu::moveBack() {
    //TODO: IMPORTANT!! after any change in menu items update next data too
    switch (currentMenu) {
        case mGo:
            currentMenu = mMain;
            currentPosition = 0;
            break;
        case mProfiles:
            currentMenu = mMain;
            currentPosition = 6;
            break;
        case mSettings:
            currentMenu = mMain;
            currentPosition = 8;
            break;
    }
}

int MainMenu::getCurrentLabelId() {
    switch (currentMenu) {
        case mMain:
            return mMainLabels[currentPosition];
        case mGo:
            return mGoLabels[currentPosition];
        case mProfiles:
            return mProfilesLabels[currentPosition];
        case mSettings:
            return mSettingsLabels[currentPosition];
        default:
            return lUndefined;
    }
}

void MainMenu::move(bool up) {
    currentPosition += up ? -1 : 1;
    int max = getMenuItemsCount(currentMenu);
    currentPosition = currentPosition < 0 ? 0 : (currentPosition >= max ? max - 1 : currentPosition);
}

int MainMenu::getMenuItemsCount(int menu) {
    switch (menu) {
        case mMain:
            return mMainLength;
        case mGo:
            return mGoLength;
        case mProfiles:
            return mProfilesLength;
        case mSettings:
            return mSettingsLength;
        default:
            return 0;
    }
}

void MainMenu::setCallback(CallbackFunctions &callbackFunctions) {
    this->callbackFunctions = callbackFunctions;
}

void MainMenu::processLogic(ButtonState buttonState) {
    if (!valueSelector.getDone()) {
        valueSelector.onButtonClick(buttonState);
        if (valueSelector.updateAndGetDone()) {
            processValueSelectorResult();
            initialized = false;
        }
    } else {
        if (currentMenu == mMain && buttonState.button == Button::SET) {
            Profile *profile = &callbackFunctions.getCurrentProfile(); //if compiler says that it's wrong - it's a lie
            if (mMainLabels[currentPosition] == lBulb) {
                callbackFunctions.setBulbOn(true);
                bulbMillis = millis();
                currentMenu = mBulb;
            } else if (mMainLabels[currentPosition] == lShutterSpeed) {
                valueSelector.startEvSelect(*profile, true);
            } else if (mMainLabels[currentPosition] == lDelay) {
                valueSelector.startEvSelect(*profile, false);
            } else if (mMainLabels[currentPosition] == lGroup) {
                valueSelector.startValueSelect(profile->group, 9000, 1);
            } else if (mMainLabels[currentPosition] == lShootsLimit) {
                valueSelector.startValueSelect(profile->countLimit, 9000, 0);
            } else if (mMainLabels[currentPosition] == lReset) {
                callbackFunctions.resetCurrentProfile();
            } else if (mMainLabels[currentPosition] == lGo) {
                currentMenu = mGo;
                currentPosition = 0;
                shootingPaused = false;
                callbackFunctions.setShootingOn(true);
            }
            initialized = false;
        } else if (currentMenu == mBulb && buttonState.button == Button::SET) {
            currentMenu = mMain;
            callbackFunctions.setBulbOn(false);
            initialized = false;
        } else if (currentMenu == mGo && buttonState.button == Button::SET) {
            switch (mGoLabels[currentPosition]) {
                case lInfo:
                    currentMenu = mInfo;
                    infoMillis = millis();
                    infoStage = InfoStage::COUNT;
                    break;
                case lDone:
                    callbackFunctions.setShootingOn(false);
                    shootingPaused = false;
                    currentMenu = mMain;
                    currentPosition = 0;
                    break;
                case lProfile:
                    valueSelector.startValueSelect(
                            callbackFunctions.getCurrentProfileId(), callbackFunctions.getProfilesCount(), 1, true);
                    break;
                case lPause:
                    shootingPaused = !shootingPaused;
                    callbackFunctions.setShootingPaused(shootingPaused);
                    break;
            }
            initialized = false;
        } else if (currentMenu == mProfiles && buttonState.button == Button::SET) {
            valueSelector.startValueSelect(1, callbackFunctions.getProfilesCount(), 1, true);
            initialized = false;
        } else if (currentMenu == mSettings && buttonState.button == Button::SET) {
            Settings *settings = callbackFunctions.getSettings();
            switch (mSettingsLabels[currentPosition]) {
                case lDisplayTimeout:
                    valueSelector.startTimeSelect(settings->displayTimeout, MAX_DISPLAY_TIMEOUT,
                                                  MIN_DISPLAY_TIMEOUT, true);
                    break;
                case lHardReset:
                    valueSelector.startBoolSelect(false);
                    break;
                case lFullLabels:
                    valueSelector.startBoolSelect(!settings->shortLabels);
                    break;
                case lAutoSaveProfile:
                    valueSelector.startBoolSelect(settings->saveCurrentProfile);
                    break;
                case lReadOnStart:
                    valueSelector.startBoolSelect(settings->loadCurrentProfileOnStart);
                    break;
                case lShowSelectHelp:
                    valueSelector.startBoolSelect(settings->showEvSelectHelp);
                    break;
            }
            initialized = false;
        } else if (currentMenu == mInfo) {
            currentMenu = mGo;
            initialized = false;
        } else if (currentMenu == mDetailedInfo) {
            Profile *profile = &callbackFunctions.getCurrentProfile();
            infoStage = getNextDetailedInfoStage(infoStage, *profile);
            if (infoStage == InfoStage::_END) {
                currentMenu = mProfiles;
            }
            initialized = false;
        }
    }
}

void MainMenu::setDisplayLabelId(int id) {
    Label label = Label();
    label.string = display.getLabelString(id);
    display.setLabel(label);
}

void MainMenu::processValueSelectorResult() {
    Profile *profile = &callbackFunctions.getCurrentProfile();
    if (currentMenu == mMain) {
        switch (mMainLabels[currentPosition]) {
            case lShutterSpeed:
                valueSelector.updateProfile(*profile, true);
                break;
            case lDelay:
                valueSelector.updateProfile(*profile, false);
                break;
            case lGroup:
                profile->group = (int) valueSelector.getValue();
                break;
            case lShootsLimit:
                profile->countLimit = (int) valueSelector.getValue();
                break;
        }
        callbackFunctions.onCurrentProfileChange();
    } else if (currentMenu == mGo && mGoLabels[currentPosition] == lProfile) {
        int profileId = (int) valueSelector.getValue();
        if (profileId != CURRENT_VALUE) {
            callbackFunctions.setProfileId(profileId);
        }
    } else if (currentMenu == mProfiles) {
        int id = (int) valueSelector.getValue();
        if (id == CURRENT_VALUE) id = 0;
        switch (mProfilesLabels[currentPosition]) {
            case lLoad:
                callbackFunctions.setProfileId(id);
                break;
            case lSave:
                callbackFunctions.saveCurrentProfileToId(id);
                break;
            case lInfo:
                currentMenu = mDetailedInfo;
                detailedInfoProfile = callbackFunctions.getProfileById(id);
                infoStage = InfoStage::EV_LABEL;
                infoMillis = millis();
                break;
        }
    } else if (currentMenu == mSettings) {
        bool binaryValueSelectorValue = static_cast<bool>(valueSelector.getValue());
        if (mSettingsLabels[currentPosition] == lHardReset && binaryValueSelectorValue) {
            callbackFunctions.hardReset();
        } else {
            Settings *settings = callbackFunctions.getSettings();
            switch (mSettingsLabels[currentPosition]) {
                case lDisplayTimeout:
                    settings->displayTimeout = valueSelector.getValue();
                    break;
                case lFullLabels:
                    settings->shortLabels = !binaryValueSelectorValue;
                    break;
                case lAutoSaveProfile:
                    settings->saveCurrentProfile = binaryValueSelectorValue;
                    break;
                case lReadOnStart:
                    settings->loadCurrentProfileOnStart = binaryValueSelectorValue;
                    break;
                case lShowSelectHelp:
                    settings->showEvSelectHelp = binaryValueSelectorValue;
                    break;
            }
            callbackFunctions.onSettingsChange();
        }
    }
}

InfoStage MainMenu::getNextInfoStage(InfoStage currentStage, bool cyclic) {
    switch (currentStage) {
        case InfoStage::COUNT:
            return InfoStage::OF_LABEL;
        case InfoStage::OF_LABEL:
            return InfoStage::FULL_COUNT;
        default:
            return cyclic ? InfoStage::COUNT : InfoStage::_END;
    }
}

InfoStage MainMenu::getNextDetailedInfoStage(InfoStage currentStage, Profile &profile) {
    switch (currentStage) {
        case InfoStage::EV_LABEL:
            return InfoStage::EV_MODE;
        case InfoStage::EV_MODE:
            return InfoStage::EV;
        case InfoStage::EV:
            return profile.evMode == MODE_NONE ? InfoStage::DELAY_LABEL : InfoStage::END_EV;
        case InfoStage::END_EV:
            return InfoStage::DELAY_LABEL;
        case InfoStage::DELAY_LABEL:
            return InfoStage::DELAY_MODE;
        case InfoStage::DELAY_MODE:
            return InfoStage::DELAY;
        case InfoStage::DELAY:
            return profile.delayMode == MODE_NONE ? InfoStage::GROUP_LABEL : InfoStage::END_DELAY;
        case InfoStage::END_DELAY:
            return InfoStage::GROUP_LABEL;
        case InfoStage::GROUP_LABEL:
            return InfoStage::GROUP;
        case InfoStage::GROUP:
            return InfoStage::COUNT_LABEL;
        case InfoStage::COUNT_LABEL:
            return InfoStage::COUNT;
        default:
            return InfoStage::_END;
    }
}

void MainMenu::setTimeSelectHelp(bool value) {
    valueSelector.setShowHelp(value);
}
