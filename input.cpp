#include <string.h>
#include <dinput.h>

#include "input.h"

static IDirectInputDevice8 *g_keyboard;
static IDirectInputDevice8 *g_mouse;

static char g_keyboard_state[256];
static DIMOUSESTATE2 g_mouse_state;

void input_init(IDirectInputDevice8 *keyboard, IDirectInputDevice8 *mouse) {
    g_keyboard = keyboard;
    g_mouse = mouse;

    memset(&g_keyboard_state, 0, sizeof(g_keyboard_state));
    memset(&g_mouse_state, 0, sizeof(g_mouse_state));
}

void input_poll() {
    HRESULT result;

    result = g_keyboard->GetDeviceState(sizeof(g_keyboard_state), &g_keyboard_state);
    if (result != DI_OK) {
        memset(&g_keyboard_state, 0, sizeof(g_keyboard_state));
        g_keyboard->Acquire();
    }

    result = g_mouse->GetDeviceState(sizeof(g_mouse_state), &g_mouse_state);
    if (result != DI_OK) {
        memset(&g_mouse_state, 0, sizeof(g_mouse_state));
        g_mouse->Acquire();
    }
}

bool key_down(int key) {
    return (g_keyboard_state[key] & 0x80) != 0;
}

bool button_down(int button) {
    return (g_mouse_state.rgbButtons[button] & 0x80) != 0;
}

float mouse_dx() {
    return g_mouse_state.lX;
}

float mouse_dy() {
    return g_mouse_state.lY;
}
