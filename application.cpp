#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "framework.h"

void on_setup(config *cfg) {
    cfg->title = "Demo";
}

void on_ready() {
}

void on_loss() {
}

void on_reset(int width, int height) {
    printf("%d, %d\n", width, height);
}

void on_render(int dtime) {
    g_direct3d->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffeeeeee, 1.0f, 0);
    g_direct3d->Present(NULL, NULL, NULL, NULL);
}
