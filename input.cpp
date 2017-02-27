#include <string.h>
#include <winerror.h>
#include <dinput.h>
#include "input.h"

Input::Input(IDirectInputDevice8 *keyboard, IDirectInputDevice8 *mouse) {
    m_keyboard = keyboard;
    m_mouse = mouse;

    memset(&m_keyboard_state, 0, sizeof(m_keyboard_state));
    memset(&m_mouse_state, 0, sizeof(m_mouse_state));
}

void Input::poll() {
    HRESULT result;

    result = m_keyboard->GetDeviceState(sizeof(m_keyboard_state),
                                        &m_keyboard_state);
    if (FAILED(result)) {
        memset(&m_keyboard_state, 0, sizeof(m_keyboard_state));
        m_keyboard->Acquire();
    }

    result = m_mouse->GetDeviceState(sizeof(m_mouse_state), &m_mouse_state);
    if (FAILED(result)) {
        memset(&m_mouse_state, 0, sizeof(m_mouse_state));
        m_mouse->Acquire();
    }
}

bool Input::key_down(int key) {
    return (m_keyboard_state[key] & 0x80) != 0;
}

bool Input::button_down(int button) {
    return (m_mouse_state.rgbButtons[button] & 0x80) != 0;
}

float Input::mouse_dx() {
    return m_mouse_state.lX;
}

float Input::mouse_dy() {
    return m_mouse_state.lY;
}
