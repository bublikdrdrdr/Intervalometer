//
// Created by bublik on 10-Aug-18.
//

#include <Arduino.h>
#include "Buttons.h"

Buttons::Buttons(char upPin, char downPin, char setPin, bool analogMode) {
    upButtonPin = upPin;
    downButtonPin = downPin;
    setButtonPin = setPin;
    this->analogMode = analogMode;
    initialize();
}

void Buttons::initialize() {
    pinMode(upButtonPin, INPUT_PULLUP);
    pinMode(downButtonPin, INPUT_PULLUP);
    pinMode(setButtonPin, INPUT_PULLUP);
}

ButtonState previousState;
State previousEventState = State::NO_STATE;
unsigned long buttonTimer;

ButtonState Buttons::getState() {
    Button currentButton = getCurrentButton();
    if (currentButton == Button::NO_BUTTON) {
        previousState.state = State::NO_STATE;
        previousEventState = State::NO_STATE;
    } else {
        if (currentButton != previousState.button) {
            buttonTimer = millis();
            previousState.state = State::NO_STATE;
            previousEventState = State::NO_STATE;
        } else {
            unsigned long difference = millis() - buttonTimer;
            if (previousEventState == State::NO_STATE && difference > CLICK_TIME) {
                previousState.state = State::CLICK;
                previousEventState = State::CLICK;
                buttonTimer = millis();
            } else if ((previousEventState == State::CLICK && difference > PRESS_TIME)
                       || (previousEventState == State::PRESS && difference > PRESS_REPEAT_TIME)) {
                previousState.state = State::PRESS;
                previousEventState = State::PRESS;
                buttonTimer = millis();
            } else {
                previousState.state = State::NO_STATE;
            }
        }
    }
    previousState.button = currentButton;
    return previousState;
}

Button Buttons::getCurrentButton() {
    char setVar = 0, upVar = 0, downVar = 0;
    if (isPinTriggered(setButtonPin))
        setVar++;
    if (isPinTriggered(upButtonPin))
        upVar++;
    if (isPinTriggered(downButtonPin))
        downVar++;
    if (setVar + upVar + downVar > 1)
        return Button::MULTI;
    if (setVar != 0)
        return Button::SET;
    if (upVar != 0)
        return Button::UP;
    if (downVar != 0)
        return Button::DOWN;
    return Button::NO_BUTTON;
}

bool Buttons::isPinTriggered(char pin) {
    if (analogMode) {
        return analogRead(pin) < 500;
    } else {
        return !digitalRead(pin);
    }
}
