//
// Created by bublik on 25-Aug-18.
//

#include <Arduino.h>
#include "ValueSelector.h"
#include "../TimeTools.h"
#include "../Labels.h"
#include "../NumberPrinter.h"

ValueSelector::ValueSelector(Display &display, bool showHelp, bool systemDefinesStepsMode, bool defaultStepsMode)
        : systemDefinesStepsMode(systemDefinesStepsMode), stepsMode(defaultStepsMode), display(display),
          showHelp(showHelp) {
    if (showHelp) helpLabelLastMillis = millis();
}

void ValueSelector::onButtonClick(ButtonState button) {
    lastButton = button;
}

void ValueSelector::updateProfile(Profile &profile, bool shutterUpdate) {
    if (shutterUpdate) {
        profile.ev = value;
        profile.endEv = endValue;
        profile.evMode = mode;
        profile.evStepsMode = stepsMode;
    } else {
        profile.delay = value;
        profile.endDelay = endValue;
        profile.delayMode = mode;
        profile.delayStepsMode = stepsMode;
    }
}

bool ValueSelector::isButtonProcessed() {
    if (lastButton.button == Button::NO_BUTTON || lastButton.button == Button::MULTI) return false;
    if (lastButton.state == State::NO_STATE) return false;
    if (lastButton.button == Button::UP || lastButton.button == Button::DOWN) {
        bool increase = lastButton.button == Button::DOWN;
        if (selectorMode == SelectorMode::EV) {
            switch (stage) {
                case EvSelectorStage::MODE:
                    mode += increase ? 1 : -1;
                    mode = mode < 0 ? 0 : (mode > MODE_COS ? MODE_COS : mode);
                    break;
                case EvSelectorStage::STEPS_MODE:
                    stepsMode = increase;
                    break;
                case EvSelectorStage::VALUE:
                    TimeTools::change(value, stepsMode, increase, lastButton.state == State::PRESS);
                    break;
                case EvSelectorStage::FIRST_VALUE:
                    TimeTools::change(value, stepsMode, increase, lastButton.state == State::PRESS);
                    break;
                case EvSelectorStage::END_VALUE:
                    TimeTools::change(endValue, stepsMode, increase, lastButton.state == State::PRESS);
                    break;
            }
        } else if (selectorMode == SelectorMode::NUMBER || selectorMode == SelectorMode::NUMBER_CURRENT) {
            changeValue(increase, lastButton.state == State::PRESS);
        } else if (selectorMode == SelectorMode::TIME || selectorMode == SelectorMode::TIME_NO) {
            changeTime(increase, lastButton.state == State::PRESS);
        } else if (selectorMode == SelectorMode::BOOL){
            value = increase?1:0;
        }
        lastButton.state = State::NO_STATE;
        return true;
    }
    if (lastButton.button == Button::SET) {
        increaseStage();
        lastButton.state = State::NO_STATE;
        return true;
    }
    lastButton.state = State::NO_STATE;
    return false;
}

void ValueSelector::increaseStage() {
    if (selectorMode != SelectorMode::EV) {
        stage = EvSelectorStage::DONE;
    } else {
        switch (stage) {
            case EvSelectorStage::MODE:
                stage = systemDefinesStepsMode ? EvSelectorStage::FIRST_VALUE : EvSelectorStage::STEPS_MODE;
                break;
            case EvSelectorStage::STEPS_MODE:
                stage = mode == MODE_NONE ? EvSelectorStage::VALUE : EvSelectorStage::FIRST_VALUE;
                break;
            case EvSelectorStage::VALUE:
                stage = EvSelectorStage::DONE;
                break;
            case EvSelectorStage::FIRST_VALUE:
                stage = EvSelectorStage::END_VALUE;
                break;
            case EvSelectorStage::END_VALUE:
                stage = stage = EvSelectorStage::DONE;
                break;
            default:
                stage = EvSelectorStage::DONE;
        }
    }
}

bool ValueSelector::updateAndGetDone() {
    if (stage == EvSelectorStage::DONE) return true;
    if (isButtonProcessed()) {
        helpLabelLastMillis = millis();
        helpLabelFlag = false;
        labelChanged = true;
    } else if (showHelp && millis() - helpLabelLastMillis > HELP_ANIMATION_MILLIS) {
        helpLabelLastMillis = millis();
        helpLabelFlag = !helpLabelFlag;
        labelChanged = true;
    }
    if (!labelChanged) return false;
    labelChanged = false;
    Label label = Label();
    if (helpLabelFlag && showHelp && selectorMode == SelectorMode::EV) {
        label.string = display.getLabelString(getStageHelpLabelId(stage));
    } else if (stage == EvSelectorStage::MODE) {
        label.string = display.getLabelString(ProfileTools::getModeLabel(mode));
    } else if (stage == EvSelectorStage::STEPS_MODE) {
        label.string = display.getLabelString(stepsMode ? lSteps : lTime);
    } else if (stage == EvSelectorStage::VALUE || stage == EvSelectorStage::FIRST_VALUE ||
               stage == EvSelectorStage::END_VALUE) {
        if (selectorMode == SelectorMode::EV) {
            label = stepsMode ? (NumberPrinter::formatSteps(stage == EvSelectorStage::END_VALUE ? endValue : value)) :
                    NumberPrinter::formatTime(stage == EvSelectorStage::END_VALUE ? endValue : value, TimeFormat::FULL);
            label.setArrayPointer();
            label.string = label.stringSymbols;
        } else if (selectorMode == SelectorMode::NUMBER_CURRENT || selectorMode == SelectorMode::NUMBER){
            if (value == CURRENT_VALUE){
                label.string = display.getLabelString(lCurrent);
            } else {
                label = NumberPrinter::formatTime(value*1000, TimeFormat::SECONDS);
                label.setArrayPointer();
                label.string = label.stringSymbols;
            }
        } else if (selectorMode == SelectorMode::TIME || selectorMode == SelectorMode::TIME_NO) {
            if (value == NO_TIME_VALUE){
                label.string = display.getLabelString(lNone);
            } else {
                label = NumberPrinter::formatTime(value, TimeFormat::FULL);
                label.setArrayPointer();
                label.string = label.stringSymbols;
            }
        } else if (selectorMode == SelectorMode::BOOL){
            label.string = display.getLabelString(value==0?lNo:lYes);
        }
    }
    display.setLabel(label);
    return stage==EvSelectorStage::DONE;
}

int ValueSelector::getStageHelpLabelId(EvSelectorStage stage) {
    switch (stage) {
        case EvSelectorStage::MODE:
            return lMode;
        case EvSelectorStage::STEPS_MODE:
            return lSteps;
        case EvSelectorStage::VALUE:
            return lTime;
        case EvSelectorStage::FIRST_VALUE:
            return lStart;
        case EvSelectorStage::END_VALUE:
            return lEnd;
        default:
            return lUndefined;
    }
}

void ValueSelector::changeValue(bool increase, bool longPress) {
    if (changeSpecialValue(increase, CURRENT_VALUE, SelectorMode::NUMBER_CURRENT)) return;
    long fraction = 1;
    while (fraction > 0 && fraction * 10 < value) {
        fraction *= 10;
    }
    long milliFraction = fraction / 10;
    if (milliFraction < 1) milliFraction = 1;
    value += (increase ? 1 : -1) * (longPress ? fraction : milliFraction);
    value = value < minValue ? minValue : (value > maxValue ? maxValue : value);
}

void ValueSelector::changeTime(bool increase, bool longPress) {
    if (changeSpecialValue(increase, NO_TIME_VALUE, SelectorMode::TIME_NO)) return;
    TimeTools::change(value, false, increase, longPress);
    value = value < minValue ? minValue : (value > maxValue ? maxValue : value);
}

bool ValueSelector::changeSpecialValue(bool increase, long specialValue, SelectorMode specialMode) {
    if (selectorMode == specialMode) {
        if (value == specialValue) {
            if (increase) value = minValue;
            return true;
        } else if (value == minValue && !increase) {
            value = specialValue;
            return true;
        }
    }
    return false;
}

void ValueSelector::startEvSelect() {
    selectorMode = SelectorMode::EV;
    stage = EvSelectorStage::MODE;
    mode = MODE_NONE;
    value = 0;
    endValue = 0;
    stepsMode = true;
    labelChanged = true;
}

void ValueSelector::startEvSelect(Profile &profile, bool shutterUpdate, bool systemDefinesStepsMode,
                                  bool defaultStepsMode) {
    this->systemDefinesStepsMode = systemDefinesStepsMode;
    selectorMode = SelectorMode::EV;
    stage = EvSelectorStage::MODE;
    value = shutterUpdate ? profile.ev : profile.delay;
    endValue = shutterUpdate ? profile.endEv : profile.endDelay;
    mode = shutterUpdate ? profile.evMode : profile.delayMode;
    stepsMode = systemDefinesStepsMode ? defaultStepsMode : (shutterUpdate ? profile.evStepsMode
                                                                           : profile.delayStepsMode);
    labelChanged = true;
}

void ValueSelector::startTimeSelect(long value, long maxValue, long minValue, bool noValue) {
    this->value = value;
    this->maxValue = maxValue;
    this->minValue = minValue < 0 ? 0 : minValue;
    this->selectorMode = noValue ? SelectorMode::TIME_NO : SelectorMode::TIME;
    stage = EvSelectorStage::VALUE;
    labelChanged = true;
}

void ValueSelector::startValueSelect(long value, long maxValue, long minValue, bool currentValueEnabled) {
    stage = EvSelectorStage::VALUE;
    this->value = value;
    this->minValue = minValue < 0 ? 0 : minValue;
    this->maxValue = maxValue;
    this->selectorMode = currentValueEnabled ? SelectorMode::NUMBER_CURRENT : SelectorMode::NUMBER;
    labelChanged = true;
}

void ValueSelector::startBoolSelect(bool value) {
    stage = EvSelectorStage::VALUE;
    selectorMode = SelectorMode::BOOL;
    this->value = value?1:0;
    minValue = 0;
    maxValue = 1;
    labelChanged = true;
}

void ValueSelector::setShowHelp(bool value) {
    showHelp = value;
}

bool ValueSelector::getDone() {
    return stage == EvSelectorStage::DONE;
}

long ValueSelector::getValue() {
    return value;
}
