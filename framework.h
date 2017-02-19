#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <d3d9.h>
#include <dinput.h>

extern IDirect3DDevice9 *g_direct3d;
extern IDirectInputDevice8 *g_keyboard;
extern IDirectInputDevice8 *g_mouse;

void on_config(const char **title, int *width, int *height);

void on_setup(int width, int height);
void on_teardown();

void on_loss();
void on_reset(int width, int height);

void on_render(int dtime);

#endif
