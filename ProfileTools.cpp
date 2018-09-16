//
// Created by bublik on 11-Aug-18.
//

#include <Arduino.h>
#include "ProfileTools.h"
#include "TimeTools.h"
#include "Labels.h"

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398

float ProfileTools::toModePercent(float percent, char mode) {
    switch (mode) {
        case MODE_LINEAR:
            return percent;
        case MODE_SQUARE:
            return percent * percent;
        case MODE_SQUARE_INVERTED:
            return (1 - percent) * (1 - percent);
        case MODE_SIN:
            return sin(percent * HALF_PI);
        case MODE_COS:
            return cos(percent * HALF_PI);
        default:
            return 0;
    }
}

unsigned long ProfileTools::calculateTime(long startValue, long endValue, bool stepMode, char mode, float percent) {
    if (stepMode) {
        startValue = TimeTools::stepsToTime(startValue);
        endValue = TimeTools::stepsToTime(endValue);
    }
    percent = toModePercent(percent, mode);
    long adjustment = endValue - startValue;
    return (unsigned long) (startValue + (long) (adjustment * percent));
}

unsigned long ProfileTools::calculateProfileEv(Profile profile, int count) {
    return calculateTime(profile.ev, profile.endEv, profile.evStepsMode, profile.evMode,
                         count / (float) profile.countLimit);
}

unsigned long ProfileTools::calculateProfileDelay(Profile profile, int count) {
    return calculateTime(profile.delay, profile.endDelay, profile.delayStepsMode, profile.delayMode,
                         count / (float) profile.countLimit);
}

int ProfileTools::getModeLabel(char mode) {
    switch (mode) {
        case MODE_NONE:
            return lNone;
        case MODE_LINEAR:
            return lLinear;
        case MODE_SQUARE:
            return lSquare;
        case MODE_SQUARE_INVERTED:
            return lSquareInverted;
        case MODE_SIN:
            return lSine;
        case MODE_COS:
            return lCosine;
        default:
            return lUndefined;
    }
}

Profile::Profile(bool evStepsMode, bool delayStepsMode, char evMode, char delayMode, int countLimit,
                 int group, long ev, long endEv, long delay, long endDelay) : evStepsMode(evStepsMode),
                                                                              delayStepsMode(delayStepsMode),
                                                                              evMode(evMode), delayMode(delayMode),
                                                                              countLimit(countLimit), group(group),
                                                                              ev(ev), endEv(endEv), delay(delay),
                                                                              endDelay(endDelay) {}

Profile::Profile() : Profile(true, true, MODE_NONE, MODE_NONE, 0, 1, 0, 0, 0, 0) {}
