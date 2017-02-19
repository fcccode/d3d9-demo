#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "framework.h"
#include "input.h"

void on_config(const char **title, int *width, int *height) {
    *title = "Demo";
}

void on_setup(int width, int height) {
    printf("on_setup: %d, %d\n", width, height);
    input_init(g_keyboard, g_mouse);
}

void on_teardown() {

}

void on_loss() {
}

void on_reset(int width, int height) {
    printf("on_reset: %d, %d\n", width, height);
}

void on_render(int dtime) {
    g_direct3d->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffeeeeee, 1.0f, 0);
    g_direct3d->Present(NULL, NULL, NULL, NULL);
}
