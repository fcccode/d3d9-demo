#ifndef INPUT_H
#define INPUT_H

#include <dinput.h>

#define KEY_A DIK_A
#define KEY_D DIK_D
#define KEY_S DIK_S
#define KEY_W DIK_W

#define BUTTON_L 0
#define BUTTON_R 1

class Input {
public:
    Input(IDirectInputDevice8 *keyboard, IDirectInputDevice8 *mouse);
    void poll();
    bool key_down(int key);
    bool button_down(int button);
    float mouse_dx();
    float mouse_dy();

private:
    IDirectInputDevice8 *m_keyboard;
    IDirectInputDevice8 *m_mouse;
    char m_keyboard_state[256];
    DIMOUSESTATE2 m_mouse_state;
};

#endif
