#include <d3d9.h>
#include <d3dx9.h>
#include "framework.h"
#include "camera.h"
#include "crate.h"
#include "input.h"
#include "skybox.h"
#include "terrain.h"
#include "vertex.h"
#include "error.h"

static IDirect3DDevice9 *g_direct3d;
static IDirectInputDevice8 *g_keyboard;
static IDirectInputDevice8 *g_mouse;

static Input *g_input;
static Camera *g_camera;
static Terrain *g_terrain;
static Skybox *g_skybox;
static Crate *g_crate;

void on_config(const char **title, int *width, int *height) {
    *title = "Snowman";
}

void on_setup(IDirect3DDevice9 *direct3d, int width, int height,
              IDirectInputDevice8 *keyboard, IDirectInputDevice8 *mouse) {
    g_direct3d = direct3d;
    g_keyboard = keyboard;
    g_mouse    = mouse;

    vertex_decl_init(g_direct3d);

    g_input   = new Input(g_keyboard, g_mouse);
    g_camera  = new Camera(width * 1.0f / height);
    g_terrain = new Terrain(g_direct3d);
    g_skybox  = new Skybox(g_direct3d);
    g_crate   = new Crate(direct3d);
}

void on_teardown() {
    delete g_terrain;
    delete g_camera;
    delete g_input;
    delete g_skybox;
    delete g_crate;

    vertex_decl_free();
}

void on_loss() {
    g_terrain->on_lost();
    g_skybox->on_lost();
}

void on_reset(int width, int height) {
    g_terrain->on_reset();
    g_skybox->on_reset();
    g_camera->on_reset(width * 1.0f / height);
}

void on_render(float dtime) {
    g_camera->travel(g_input, g_terrain, dtime);

    g_direct3d->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                      0xff808080, 1.0f, 0);
    OK_3D(g_direct3d->BeginScene());

    g_skybox->render(g_direct3d, g_camera->get_pos(),
                     g_camera->get_view_proj());
    g_terrain->render(g_direct3d, g_camera->get_view_proj());
    g_crate->render(g_direct3d, g_camera->get_pos(), g_camera->get_view_proj());

    OK_3D(g_direct3d->EndScene());
    g_direct3d->Present(NULL, NULL, NULL, NULL);
}
