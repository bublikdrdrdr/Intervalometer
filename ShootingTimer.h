//
// Created by bublik on 16-Aug-18.
//

#ifndef INTERVALOMETER_SHOOTINGTIMER_H
#define INTERVALOMETER_SHOOTINGTIMER_H

#include "ProfileTools.h"

#define BETWEEN_GROUP_DELAY 500
#define CHANGE_PROFILE_LAG 80

#define ENABLE_BLINK_LED true
#define BLINK_LED_A_PORT 11
#define BLINK_LED_K_PORT 10
#define LED_BLINK_MILLIS 1

enum class ShootingState {
    OFF,
    ON,
    PAUSED,
    BULB
};

class ShootingTimer {
public:
    ShootingTimer(char pin, bool invertMode = true);
    void setState(ShootingState state);
    void setProfile(Profile profile);
    int getCount();
    ShootingState getState();
    void update();
private:
    char pin;
    bool invertedMode;
    ShootingState state;
    Profile profile;
    bool pinActive = false;
    unsigned long lastMillis;
    int groupCounter;
    int shootingCounter;
    unsigned long currentShootingDelay;
    unsigned long currentShootingDuration;
    void setCameraOn(bool value);
    void updateShootingData(int count);
    void setupBlinkLed();
    void blinkLed();
};


#endif //INTERVALOMETER_SHOOTINGTIMER_H
