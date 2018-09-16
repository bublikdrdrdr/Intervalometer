//
// Created by bublik on 10-Aug-18.
//

#ifndef INTERVALOMETER_DISPLAY_H
#define INTERVALOMETER_DISPLAY_H

#include "TM1637Display.h"

#define DEFAULT_ANIMATION_DELAY 250
#define BLINKING_DELAY 500
#define BLINKING_DOTS_DELAY 1000
#define FAST_BLINKING_DOTS_DELAY 80

#define HARDWARE_DISPLAY_LENGTH 4
#define MAX_DISPLAY_NUMBER 9999

enum class DotsMode {
    OFF,
    BLINKING,
    BLINKING_FAST,
    ON
};

struct Label {
    Label();
    //IMPORTANT!!: call this after function returned
    void setArrayPointer();
    char stringSymbols[HARDWARE_DISPLAY_LENGTH+1];
    const char *string = stringSymbols;
    DotsMode mode = DotsMode::OFF;
};

class Display {
public:
    Display(char clockPin, char dioPin) : hardwareDisplay(clockPin, dioPin) {};
    void setFixedLength(bool fixed);
    void update(bool force = false);
    void setLabel(Label &label);
    void setAnimationDelay(unsigned long delay);
    void setBlinking(bool blinking);
    static void initializeLabel(Label &rawLabel);
    const char *getLabelString(int label);
    void setDisplayOn(bool value);
private:
    TM1637Display hardwareDisplay;
    bool fixedLength = true;
    bool blinking = false;
    int cursorPosition = 0;
    int currentLabelLength = 0;
    bool valid = false;
    Label label;
    bool on = true;
    unsigned long animationDelay = DEFAULT_ANIMATION_DELAY;
};


#endif //INTERVALOMETER_DISPLAY_H
