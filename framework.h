#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <d3d9.h>
#include <dinput.h>

void on_config(const char **title, int *width, int *height);

void on_setup(IDirect3DDevice9 *direct3d, int width, int height,
              IDirectInputDevice8 *keyboard, IDirectInputDevice8 *mouse);
void on_teardown();

void on_lost();
void on_reset(int width, int height);

void on_render(float dtime);

#endif
