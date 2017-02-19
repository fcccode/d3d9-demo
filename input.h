#ifndef INPUT_H
#define INPUT_H

#include <dinput.h>

#define KEY_A DIK_A
#define KEY_D DIK_D
#define KEY_S DIK_S
#define KEY_W DIK_W

#define BUTTON_L 0
#define BUTTON_R 1

void input_init(IDirectInputDevice8 *keyboard, IDirectInputDevice8 *mouse);
void input_poll();

bool key_down(int key);
bool button_down(int button);
float mouse_dx();
float mouse_dy();

#endif
