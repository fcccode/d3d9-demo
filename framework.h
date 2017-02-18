#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <d3d9.h>
#include <dinput.h>

extern IDirect3DDevice9 *g_direct3d;
extern IDirectInputDevice8 *g_keyboard;
extern IDirectInputDevice8 *g_mouse;

typedef struct {
    const char *title;
    int x, y;
    int width, height;
} config;

void on_setup(config *cfg);
void on_ready();

void on_loss();
void on_reset(int width, int height);

void on_render(int dtime);

#endif
