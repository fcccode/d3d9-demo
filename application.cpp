#include <d3d9.h>
#include <d3dx9.h>
#include "framework.h"
#include "camera.h"
#include "input.h"
#include "terrain.h"

static Input *g_input;
static Camera *g_camera;
static Terrain *g_terrain;

void on_config(const char **title, int *width, int *height) {
    *title = "Demo";
}

void on_setup(int width, int height) {
    g_input = new Input(g_keyboard, g_mouse);
    g_camera = new Camera(width * 1.0f / height);
    g_terrain = new Terrain();
}

void on_teardown() {
    delete g_input;
    delete g_camera;
    delete g_terrain;
}

void on_loss() {
}

void on_reset(int width, int height) {
    g_camera->reset(width * 1.0f / height);
}

void on_render(int dtime) {
    g_direct3d->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffeeeeee, 1.0f, 0);
    g_direct3d->Present(NULL, NULL, NULL, NULL);
}
