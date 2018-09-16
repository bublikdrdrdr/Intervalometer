//
// Created by bublik on 10-Aug-18.
//

#include <Arduino.h>
#include "Display.h"
#include "CharMapper.h"
#include "TM1637Display.h"
#include "Labels.h"

unsigned long lastBlinkingMillis;
bool blinkingFlag = false;

unsigned long lastBlinkingDotsMillis;
bool blinkingDotsFlag = false;

unsigned long lastAnimationMillis;

void Display::setLabel(Label &source) {
    valid = false;
    cursorPosition = 0;
    label.mode = source.mode;
    for (int i = 0; i < HARDWARE_DISPLAY_LENGTH + 1; i++) {
        label.stringSymbols[i] = source.stringSymbols[i];
    }
    if (source.string != source.stringSymbols) {
        label.string = source.string;
    } else {
        label.string = label.stringSymbols;
    }
    currentLabelLength = 4;
    if (fixedLength) return;
    lastAnimationMillis = millis();
    currentLabelLength = 0;
    hardwareDisplay.setBrightness(blinkingFlag ? 0 : 7);
    while (label.string[currentLabelLength] != '\0') {
        currentLabelLength++;
    }
    if (currentLabelLength>HARDWARE_DISPLAY_LENGTH) currentLabelLength += 3;
}

void Display::setFixedLength(bool fixed) {
    cursorPosition = 0;
    fixedLength = fixed;
    valid = false;
}

uint8_t emptyDisplay[] = {0,0,0,0};

void Display::update(bool force) {
    if (!on){
        hardwareDisplay.setBrightness(0, false);
        hardwareDisplay.setSegments(emptyDisplay);
        return;
    }
    if (blinking && millis() - lastBlinkingMillis > BLINKING_DELAY) {
        blinkingFlag = !blinkingFlag;
        lastBlinkingMillis = millis();
        valid = false;
    }
    if ((label.mode == DotsMode::BLINKING && millis() - lastBlinkingDotsMillis > BLINKING_DOTS_DELAY)
        || (label.mode == DotsMode::BLINKING_FAST && millis() - lastBlinkingDotsMillis > FAST_BLINKING_DOTS_DELAY)) {
        blinkingDotsFlag = !blinkingDotsFlag;
        lastBlinkingDotsMillis = millis();
        valid = false;
    } else if (label.mode == DotsMode::OFF || label.mode == DotsMode::ON) {
        if (blinkingDotsFlag != (label.mode == DotsMode::ON)) {
            valid = false;
        }
        blinkingDotsFlag = label.mode == DotsMode::ON;
    }
    if (!fixedLength && millis() - lastAnimationMillis > animationDelay) {
        lastAnimationMillis = millis();
        cursorPosition++;
        if (cursorPosition + HARDWARE_DISPLAY_LENGTH > currentLabelLength) {
            cursorPosition = 0;
        }
        valid = false;
    }

    if (!valid || force) {
        hardwareDisplay.setBrightness(blinkingFlag ? 0 : 7);
        char croppedLabel[HARDWARE_DISPLAY_LENGTH];
        bool endOfStringReached = false;
        for (int i = 0; i < HARDWARE_DISPLAY_LENGTH; i++) {
            if (i + cursorPosition < currentLabelLength && label.string[i + cursorPosition] != '\0' && !endOfStringReached) {
                croppedLabel[i] = label.string[i + cursorPosition];
            } else {
                endOfStringReached = true;
                croppedLabel[i] = ' ';
            }
        }
        uint8_t symbols[HARDWARE_DISPLAY_LENGTH];
        CharMapper::writeLabelToSymbols(croppedLabel, symbols, HARDWARE_DISPLAY_LENGTH);
        if (blinkingDotsFlag) {
            CharMapper::setDotsVisible(symbols, true, HARDWARE_DISPLAY_LENGTH);
        }
        hardwareDisplay.setSegments(symbols);
        valid = true;
    }
}

void Display::setAnimationDelay(unsigned long delay) {
    animationDelay = delay;
}

void Display::setBlinking(bool blinking) {
    this->blinking = blinking;
    if (!blinking) {
        hardwareDisplay.setBrightness(7, on);
        blinkingFlag = false;
    }
}

void Display::initializeLabel(Label &rawLabel) {
    rawLabel.mode = DotsMode::OFF;
    for (int i = 0; i < HARDWARE_DISPLAY_LENGTH; i++) {
        rawLabel.stringSymbols[i] = ' ';
    }
}

const char *Display::getLabelString(int label) {
    return Labels::getLabelById(label, !fixedLength);
}

void Display::setDisplayOn(bool value) {
    on = value;
    update(true);
}

Label::Label() {
    for (int i = 0; i < HARDWARE_DISPLAY_LENGTH; i++) {
        this->stringSymbols[i] = ' ';
    }
    this->stringSymbols[HARDWARE_DISPLAY_LENGTH] = '\0';
    setArrayPointer();
}

void Label::setArrayPointer() {
    this->string = this->stringSymbols;
}