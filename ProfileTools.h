//
// Created by bublik on 11-Aug-18.
//

#ifndef INTERVALOMETER_PROFILETOOLS_H
#define INTERVALOMETER_PROFILETOOLS_H

#define MODE_NONE 0
#define MODE_LINEAR 1
#define MODE_SQUARE 2
#define MODE_SQUARE_INVERTED 3
#define MODE_SIN 4
#define MODE_COS 5

struct Profile{
    Profile(bool evStepsMode, bool delayStepsMode, char evMode, char delayMode, int countLimit,
            int group, long ev, long endEv, long delay, long endDelay);

    Profile();

    bool evStepsMode = true;
    bool delayStepsMode = true;
    char evMode;
    char delayMode;
    int countLimit;
    int group;
    long ev;
    long endEv;
    long delay;
    long endDelay;
};

class ProfileTools {
public:
    static int getModeLabel(char mode);
    static float toModePercent(float percent, char mode);
    static unsigned long calculateTime(long startValue, long endValue, bool stepMode, char mode, float percent);
    static unsigned long calculateProfileEv(Profile profile, int count);
    static unsigned long calculateProfileDelay(Profile profile, int count);
};


#endif //INTERVALOMETER_PROFILETOOLS_H
