//
// Created by bublik on 16-Aug-18.
//

#ifndef INTERVALOMETER_MAINMENU_H
#define INTERVALOMETER_MAINMENU_H


#include "LogicCallback.h"
#include "../Buttons.h"
#include "ValueSelector.h"

#define INFO_COUNT_TIME 2000
#define INFO_LABELS_TIME 1000

#define CYCLIC_INFO_MODE true

#define MIN_DISPLAY_TIMEOUT 10000
#define MAX_DISPLAY_TIMEOUT 43200000L

#define mMain 0
#define mGo 1
#define mProfiles 2
#define mSettings 3
#define mBulb 4
#define mInfo 5
#define mDetailedInfo 6

enum class InfoStage {
    COUNT,
    OF_LABEL,
    FULL_COUNT,
    EV_LABEL,
    EV_MODE,
    EV,
    END_EV,
    DELAY_LABEL,
    DELAY_MODE,
    DELAY,
    END_DELAY,
    GROUP_LABEL,
    GROUP,
    COUNT_LABEL,
    _END
};

class MainMenu {
public:
    MainMenu(CallbackFunctions &callbackFunctions, Display &display);
    void setCallback(CallbackFunctions &callbackFunctions);
    bool updateAndGetDone(); //update loop function, if true - continue parent menu logic
    void onButtonClick(ButtonState button);
    void setTimeSelectHelp(bool value);
private:
    void formatStepsTime(long value, bool stepsMode, Label &label);
    bool updateInfoStageAndGetInitialized();
    bool updateDetailedInfoStageAndGetInitialized();
    int getStageLabelId(InfoStage infoStage);
    InfoStage getNextInfoStage(InfoStage currentStage, bool cyclic);
    InfoStage getNextDetailedInfoStage(InfoStage currentStage, Profile& profile);
    void processValueSelectorResult();
    void setDisplayLabelId(int id);
    void processLogic(ButtonState buttonState);
    ValueSelector valueSelector;
    CallbackFunctions callbackFunctions;
    void move(bool up);
    int getMenuItemsCount(int menu);
    void moveBack();
    bool moveForward();
    int getCurrentLabelId();
    int currentMenu = mMain;
    int currentPosition = 0;
    bool initialized = false;
    Display &display;
    unsigned long bulbMillis;
    unsigned long infoMillis;
    InfoStage infoStage;
    Profile detailedInfoProfile;
    bool shootingPaused = false;
};


#endif //INTERVALOMETER_MAINMENU_H
