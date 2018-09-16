//
// Created by bublik on 11-Aug-18.
//

#ifndef INTERVALOMETER_TIMETOOLS_H
#define INTERVALOMETER_TIMETOOLS_H

#include "Display.h"

#define EV_DIVIDER 3
#define MIN_EV (-4*EV_DIVIDER)
#define MAX_EV (12*EV_DIVIDER)

#define MIN_TIME 1000
#define MAX_TIME (43200000L)

class TimeTools {
public:
    static unsigned long stepsToTime(long steps);
    static void change(long &steps, bool stepsMode, bool increase, bool longPress);
private:
    static float roundTime(float seconds);
    static void limitValue(long &value, long min, long max);
};


#endif //INTERVALOMETER_TIMETOOLS_H
