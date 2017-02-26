#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "framework.h"
#include "camera.h"
#include "crate.h"
#include "input.h"
#include "light.h"
#include "skybox.h"
#include "snowman.h"
#include "terrain.h"
#include "vertex.h"
#include "error.h"

static IDirect3DDevice9 *g_direct3d;

static Input *g_input;
static Camera *g_camera;
static Crate *g_crate;
static Skybox *g_skybox;
static Snowman *g_snowman;
static Terrain *g_terrain;

void on_config(const char **title, int *width, int *height) {
    *title = "Snowman";
}

void on_setup(IDirect3DDevice9 *direct3d, int width, int height,
              IDirectInputDevice8 *keyboard, IDirectInputDevice8 *mouse) {
    g_direct3d = direct3d;

    vertex_init(g_direct3d);
    light_init();

    g_input   = new Input(keyboard, mouse);
    g_camera  = new Camera(width * 1.0f / height);
    g_crate   = new Crate(g_direct3d);
    g_skybox  = new Skybox(g_direct3d);
    g_snowman = new Snowman(g_direct3d);
    g_terrain = new Terrain(g_direct3d);
}

void on_teardown() {
    delete g_input;
    delete g_camera;
    delete g_crate;
    delete g_skybox;
    delete g_snowman;
    delete g_terrain;

    vertex_free();
    light_free();
}

void on_lost() {
    g_crate->on_lost();
    g_terrain->on_lost();
    g_skybox->on_lost();
    g_snowman->on_lost();
}

void on_reset(int width, int height) {
    g_camera->set_lens(width * 1.0f / height);

    g_crate->on_reset();
    g_terrain->on_reset();
    g_skybox->on_reset();
    g_snowman->on_reset();
}

void on_render(float dtime) {
    g_camera->travel(g_input, g_terrain, dtime);
    D3DXVECTOR3 camera_pos = g_camera->get_pos();
    D3DXMATRIX view_proj = g_camera->get_view_proj();

    OK(g_direct3d->BeginScene());

    g_skybox->render(camera_pos, view_proj);
    g_terrain->render(view_proj);
    g_crate->render(camera_pos, view_proj);
    g_snowman->render(camera_pos, view_proj);

    OK(g_direct3d->EndScene());
    g_direct3d->Present(NULL, NULL, NULL, NULL);
}
