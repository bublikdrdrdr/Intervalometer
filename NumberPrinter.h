//
// Created by bublik on 11-Aug-18.
//

#ifndef INTERVALOMETER_NUMBERPRINTER_H
#define INTERVALOMETER_NUMBERPRINTER_H

#include "Display.h"

enum class TimeFormat{
    SECONDS,
    ROUNDED_HOURS,
    FULL
};

class NumberPrinter {
public:
    static Label formatTime(long millis, TimeFormat format);
    static void printDigits(long number, int offset, int length, char * label, int lastZeroIndex);
    static Label formatSteps(long steps);
    static void printOutOfBoundsValue(Label *label, char symbol);

private:
    static int separateBy60AndGetFull(long value);
};


#endif //INTERVALOMETER_NUMBERPRINTER_H
