//
// Created by bublik on 11-Aug-18.
//

#include "Labels.h"

static const char *labelPairs[] = {
        "shut", "shutter speed", "dela", "delay", "coun", "count limit", "prof", "profiles", "rese", "reset", "sett",
        "settings", "prof", "profile", "paus", "pause", "line", "linear", "sqr", "square", "del", "delete", "dto",
        "display timeout", "hres", "hard reset", "grp", "group", "curr", "current", "sqin", "square inverted", "step",
        "steps", "strt", "start", "flbl", "full labels", "asp", "auto save", "ros", "read on start", "shlp", "show help"
};

static const char *sameSizeLabels[]{
        "go", "bulb", "none", "time", "mode", "----", "sin", "cos", "load", "save", "info", "done", "end", " +- ", "of", "yes", "no"
};

const char *Labels::getLabelById(int label, bool full) {
    if (label < 50) {
        return labelPairs[label * 2 + (full ? 1 : 0)];
    } else {
        return sameSizeLabels[label - 50];
    }
}
