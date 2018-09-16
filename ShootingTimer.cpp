//
// Created by bublik on 16-Aug-18.
//

#include <Arduino.h>
#include "ShootingTimer.h"
#include "TimeTools.h"

ShootingTimer::ShootingTimer(char pin, bool invertMode) {
    this->pin = pin;
    this->invertedMode = invertMode;
    this->state = ShootingState::OFF;
    pinMode(pin, OUTPUT);
    if (ENABLE_BLINK_LED){
        setupBlinkLed();
    }
}

void ShootingTimer::setState(ShootingState state) {
    if (!((this->state == ShootingState::ON && state == ShootingState::PAUSED) ||
          (this->state == ShootingState::PAUSED && state == ShootingState::ON))) {
        lastMillis = millis();
        groupCounter = 0;
        shootingCounter = -1;
        updateShootingData(0);
    }
    this->state = state;
}

void ShootingTimer::setProfile(Profile profile) {
    this->profile = profile;
    if (state == ShootingState::ON) {
        setState(ShootingState::OFF);
        update();
        delay(CHANGE_PROFILE_LAG);
        setState(ShootingState::ON);
    } else {
        setState(state);
    }
}

void ShootingTimer::update() {
    if (state == ShootingState::OFF || state == ShootingState::PAUSED || state == ShootingState::BULB) {
        setCameraOn(state == ShootingState::BULB);
    }
    if (state != ShootingState::ON) return; //unsupported
    long difference = millis() - lastMillis;
    if (shootingCounter == -1 || (!pinActive && ((groupCounter != 0 && difference >= BETWEEN_GROUP_DELAY) ||
                                                 (groupCounter == 0 && difference >= currentShootingDelay)))) {
        if (shootingCounter == -1){
            shootingCounter = 0;
        }
        setCameraOn(true);
        lastMillis = millis();
        if (ENABLE_BLINK_LED){
            blinkLed();
        }
    } else if (pinActive && difference >= currentShootingDuration) {
        setCameraOn(false);
        groupCounter++;
        if (groupCounter >= profile.group) {
            groupCounter = 0;
            shootingCounter++;
        }
        if (profile.countLimit > 0 && shootingCounter > profile.countLimit) {
            setState(ShootingState::OFF);
        }
        lastMillis = millis();
        updateShootingData(shootingCounter);
    }
}

void ShootingTimer::setCameraOn(bool value) {
    pinMode(13, OUTPUT);
    digitalWrite(13, value?HIGH:LOW);
    pinActive = value;
    digitalWrite(pin, (value ^ invertedMode) ? HIGH : LOW);
}

void ShootingTimer::updateShootingData(int count) {
    currentShootingDelay = ProfileTools::calculateProfileDelay(profile, count);
    currentShootingDuration = ProfileTools::calculateProfileEv(profile, count);
}

int ShootingTimer::getCount() {
    return shootingCounter==-1?0:shootingCounter;
}

ShootingState ShootingTimer::getState() {
    return state;
}

void ShootingTimer::blinkLed() {
    digitalWrite(BLINK_LED_K_PORT, LOW);
    digitalWrite(BLINK_LED_A_PORT, HIGH);
    delay(LED_BLINK_MILLIS);
    digitalWrite(BLINK_LED_A_PORT, LOW);
}

void ShootingTimer::setupBlinkLed() {
    pinMode(BLINK_LED_A_PORT, OUTPUT);
    pinMode(BLINK_LED_K_PORT, OUTPUT);
    digitalWrite(BLINK_LED_A_PORT, LOW);
    digitalWrite(BLINK_LED_K_PORT, LOW);
}
