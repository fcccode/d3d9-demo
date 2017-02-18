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

bool input_key_down(int key) {
    return (g_keyboard_state[key] & 0x80) != 0;
}

bool input_button_down(int button) {
    return (g_mouse_state.rgbButtons[button] & 0x80) != 0;
}

void input_mouse_diff(float *dx, float *dy) {
    if (dx != NULL)
        *dx = (float)g_mouse_state.lX;
    if (dy != NULL)
        *dy = (float)g_mouse_state.lY;
}
