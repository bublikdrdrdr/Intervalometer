//
// Created by bublik on 10-Aug-18.
//

#ifndef INTERVALOMETER_BUTTONS_H
#define INTERVALOMETER_BUTTONS_H

enum class Button {
    NO_BUTTON,
    SET,
    UP,
    DOWN,
    MULTI
};

enum class State {
    NO_STATE,
    CLICK,
    PRESS
};

#define CLICK_TIME 20
#define PRESS_TIME 800
#define PRESS_REPEAT_TIME 200

struct ButtonState{
    Button button = Button::NO_BUTTON;
    State state = State ::NO_STATE;
};

class Buttons {
public:
    Buttons(char upPin, char downPin, char setPin, bool analogMode = true);
    ButtonState getState();
private:
    char setButtonPin;
    char upButtonPin;
    char downButtonPin;
    bool analogMode;
    void initialize();
    bool isPinTriggered(char pin);
    Button getCurrentButton();
};


#endif //INTERVALOMETER_BUTTONS_H
