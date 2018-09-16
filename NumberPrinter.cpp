//
// Created by bublik on 11-Aug-18.
//

#include "NumberPrinter.h"
#include "TimeTools.h"
#include <Arduino.h>

Label NumberPrinter::formatTime(long millis, TimeFormat format) {
    Label label = Label();
    long seconds = millis / 1000;
    switch (format) {
        case TimeFormat::SECONDS:
            if (seconds > MAX_DISPLAY_NUMBER) {
                printOutOfBoundsValue(&label, '-');
            } else {
                label.mode = DotsMode::OFF;
                printDigits(seconds, 0, HARDWARE_DISPLAY_LENGTH, label.stringSymbols, 0);
            }
            break;
        case TimeFormat::ROUNDED_HOURS:
            if (seconds < 60 * 60) {
                printDigits(separateBy60AndGetFull(seconds), HARDWARE_DISPLAY_LENGTH - 4, 4, label.stringSymbols, 2);
                label.mode = DotsMode::BLINKING;
            } else if (seconds / (60 * 60) < MAX_DISPLAY_NUMBER / 10) {
                int hoursWithOneDecimal = seconds / (60 * 6);
                if (hoursWithOneDecimal % 10 == 0 || hoursWithOneDecimal > MAX_DISPLAY_NUMBER / 10) {
                    hoursWithOneDecimal /= 10;
                    label.mode = DotsMode::OFF;
                } else {
                    label.mode = DotsMode::ON;
                }
                printDigits(hoursWithOneDecimal, HARDWARE_DISPLAY_LENGTH - 4, HARDWARE_DISPLAY_LENGTH - 1, label.stringSymbols,
                            0);
                label.stringSymbols[HARDWARE_DISPLAY_LENGTH - 1] = 'h';
            } else {
                printOutOfBoundsValue(&label, '-');
            }
            break;
        case TimeFormat::FULL:
            int full;
                                    if (seconds >= 356400L) {
                printOutOfBoundsValue(&label, '-');
                break;
            } else if (seconds >= 60 * 60) {
                full = separateBy60AndGetFull(seconds / 60);
                label.mode = DotsMode::ON;
            } else {
                full = separateBy60AndGetFull(seconds);
                label.mode = DotsMode::BLINKING;
            }
            printDigits(full, 0, 4, label.stringSymbols, 2);
            break;
        default:
            printOutOfBoundsValue(&label, '?');
            break;
    }
    return label;
}

int NumberPrinter::separateBy60AndGetFull(long value) {
    int minutes = value / 60;
    value = value - (minutes * 60);
    return minutes * 100 + value;
}

void NumberPrinter::printOutOfBoundsValue(Label *label, char symbol) {
    label->mode = DotsMode::OFF;
    for (int i = 0; i < HARDWARE_DISPLAY_LENGTH; i++) {
        label->stringSymbols[i] = symbol;
    }
}

/**
 (1234, 0, 4, ****) -> 1234
 (1234, 0, 2, ****) -> 34**
 (1234, 2, 2, ****) -> **34
 */
void NumberPrinter::printDigits(long number, int offset, int length, char *label, int lastZeroIndex) {
    for (int i = 0; i < length; i++) {
        char digit = number % 10;
        number = number / 10;
        int index = length - 1 + offset - i;
        if (number == 0 && digit == 0 && lastZeroIndex < i) {
            label[index] = ' ';
        } else {
            label[index] = '0' + digit;
        }
    }
}

Label NumberPrinter::formatSteps(long steps) {
    int value;
    unsigned long millis = TimeTools::stepsToTime(steps);
    Label label = Label();
    Display::initializeLabel(label);
    if (millis < 300) {
        value = round(1000. / millis);
        bool b = (value >= 10 || value == 0);
        printDigits(value, HARDWARE_DISPLAY_LENGTH - 2, b ? 2 : 1, label.stringSymbols, b ? 1 : 0);
        label.mode = DotsMode::ON;
    } else if (millis < 15000) {
        value = round(millis / 100.);
        printDigits(value, HARDWARE_DISPLAY_LENGTH-4, 3, label.stringSymbols, 1);
        if (value%10 == 0){
            label.stringSymbols[HARDWARE_DISPLAY_LENGTH-2] = ' ';
        } else {
            label.mode = DotsMode::ON;
        }
        label.stringSymbols[HARDWARE_DISPLAY_LENGTH-1] = '\'';
    } else {
        return formatTime(millis, TimeFormat::FULL);
    }
    return label;
}
