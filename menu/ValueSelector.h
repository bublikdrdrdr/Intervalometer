//
// Created by bublik on 25-Aug-18.
//

#ifndef INTERVALOMETER_EVVALUESELECTOR_H
#define INTERVALOMETER_EVVALUESELECTOR_H

#include "../ProfileTools.h"
#include "../Buttons.h"
#include "../Display.h"

#define HELP_ANIMATION_MILLIS 3000

#define CURRENT_VALUE (-1)
#define NO_TIME_VALUE (-1)

enum class EvSelectorStage {
    MODE,
    STEPS_MODE,
    VALUE,
    FIRST_VALUE,
    END_VALUE,
    DONE
};

enum class SelectorMode {
    EV,
    TIME,
    TIME_NO,
    NUMBER,
    NUMBER_CURRENT,
    BOOL
};

class ValueSelector {
public:
    ValueSelector(Display &display, bool showHelp, bool systemDefinesStepsMode = false, bool defaultStepsMode = true);
    void startValueSelect(long value, long maxValue = 10, long minValue = 0, bool currentValueEnabled = false);
    void startEvSelect();
    void startEvSelect(Profile &profile, bool shutterUpdate, bool systemDefinesStepsMode = false,
                       bool defaultStepsMode = true);
    void startTimeSelect(long value, long maxValue = 43200000L, long minValue = 0, bool noValue = false);
    void startBoolSelect(bool value);
    void updateProfile(Profile &profile, bool shutterUpdate);
    bool updateAndGetDone();
    bool getDone();
    void onButtonClick(ButtonState button);
    void setShowHelp(bool value);
    long getValue();
private:
    int getStageHelpLabelId(EvSelectorStage stage);
    void changeValue(bool increase, bool longPress);
    void changeTime(bool increase, bool longPress);
    bool changeSpecialValue(bool increase, long specialValue, SelectorMode specialMode);
    bool isButtonProcessed();

    void increaseStage();

    SelectorMode  selectorMode;

    bool showHelp;
    bool systemDefinesStepsMode;
    Display &display;
    ButtonState lastButton;
    unsigned long helpLabelLastMillis = 0;
    bool helpLabelFlag;

    long minValue;
    long maxValue;

    bool labelChanged = false;

    EvSelectorStage stage = EvSelectorStage::DONE;

    char mode = MODE_NONE;
    long value = 0;
    long endValue = 0;
    bool stepsMode = true;
};


#endif //INTERVALOMETER_EVVALUESELECTOR_H
