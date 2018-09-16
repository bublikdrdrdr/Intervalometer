//
// Created by bublik on 11-Aug-18.
//

#ifndef INTERVALOMETER_LABELS_H
#define INTERVALOMETER_LABELS_H

#define lShutterSpeed 0
#define lDelay 1
#define lShootsLimit 2
#define lProfiles 3
#define lReset 4
#define lSettings 5
#define lProfile 6
#define lPause 7
#define lLinear 8
#define lSquare 9
#define lDelete 10
#define lDisplayTimeout 11
#define lHardReset 12
#define lGroup 13
#define lCurrent 14
#define lSquareInverted 15
#define lSteps 16
#define lStart 17
#define lFullLabels 18
#define lAutoSaveProfile 19
#define lReadOnStart 20
#define lShowSelectHelp 21
#define lGo 50
#define lBulb 51
#define lNone 52
#define lTime 53
#define lMode 54
#define lUndefined 55
#define lSine 56
#define lCosine 57
#define lLoad 58
#define lSave 59
#define lInfo 60
#define lDone 61
#define lEnd 62
#define lBack 63
#define lOf 64
#define lYes 65
#define lNo 66

class Labels {
public:
    static const char *getLabelById(int label, bool full = false);
};


#endif //INTERVALOMETER_LABELS_H
