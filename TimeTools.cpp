//
// Created by bublik on 11-Aug-18.
//

#include <Arduino.h>
#include "TimeTools.h"
#include "NumberPrinter.h"

#define SECOND 1000L
#define MINUTE (60*SECOND)
#define HOUR (60*MINUTE)

unsigned long TimeTools::stepsToTime(long steps) {
    float ev = pow(2, steps / (float) EV_DIVIDER - (steps > 3 * EV_DIVIDER ? 0.093109404391481 : 0));
    return roundTime(ev) * 1000;
}

float TimeTools::roundTime(float seconds) {
    if (seconds < 1)
        return seconds;
    if (seconds < 4)
        return round(seconds * 10) / 10.;
    if (seconds < 15)
        return round(seconds);
    if (seconds < 120) {
        long roundedTo5Value = 5 * round(seconds / 5);
        long roundedTo2Value = 2 * round(seconds / 2);
        if (abs(seconds - roundedTo2Value) < abs(seconds - roundedTo5Value)) {
            return roundedTo2Value;
        } else {
            return roundedTo5Value;
        }
    }
    return 10 * round(seconds / 10);
}

void TimeTools::change(long &value, bool stepsMode, bool increase, bool longPress) {
                                    long sign = increase ? 1 : -1;
    if (stepsMode) {
        value += sign * (longPress ? EV_DIVIDER : 1);
        limitValue(value, MIN_EV, MAX_EV);
    } else {
        if (value < SECOND) {
            if (longPress) {
                value += sign * 100;
            } else {
                long divider = 1;
                while (value > divider * 10) divider *= 10;
                value += sign * divider;
            }
            limitValue(value, 1, SECOND);
        } else if (value < MINUTE) {
            long valueDelta = sign * (longPress ? 10 * SECOND : SECOND);
                                                value += valueDelta;
                        limitValue(value, SECOND-1, MINUTE);
                    } else if (value < 10 * MINUTE) {
            value += sign * (longPress ? MINUTE : 10*SECOND);
            limitValue(value, MINUTE -SECOND, 10 * MINUTE);
        } else if (value < HOUR){
            value += sign * (longPress ? 10*MINUTE : MINUTE);
            limitValue(value, 9 * MINUTE, HOUR);
        } else {
            value += sign * (longPress ? HOUR : 10*MINUTE);
            limitValue(value, HOUR - MINUTE, MAX_TIME);
        }
        limitValue(value, MIN_TIME, MAX_TIME);
                    }
}

void TimeTools::limitValue(long &value, long min, long max) {
    value = value<min?min:(value>max?max:value);
}
