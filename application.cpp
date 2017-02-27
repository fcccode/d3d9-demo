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
static Snowman *g_snowman0;
static Snowman *g_snowman1;
static Terrain *g_terrain;

void on_config(const char **title, int *width, int *height) {
    *title = "Snowman";
}

void on_setup(IDirect3DDevice9 *direct3d, int width, int height,
              IDirectInputDevice8 *keyboard, IDirectInputDevice8 *mouse) {
    g_direct3d = direct3d;

    vertex_init(g_direct3d);
    light_init();

    D3DXVECTOR3 camera_pos   = D3DXVECTOR3(-380.0f, 100.0f, 450.0f);
    D3DXVECTOR3 crate_center = D3DXVECTOR3(-375.0f,  97.5f, 560.0f);
    D3DXVECTOR3 snowman0_pos = D3DXVECTOR3(-305.0f, 113.0f, 560.0f);
    D3DXVECTOR3 snowman1_pos = D3DXVECTOR3(-375.0f,  99.0f, 560.0f);

    g_input    = new Input(keyboard, mouse);
    g_camera   = new Camera(width * 1.0f / height, camera_pos);
    g_crate    = new Crate(g_direct3d, crate_center);
    g_skybox   = new Skybox(g_direct3d);
    g_snowman0 = new Snowman(g_direct3d, snowman0_pos);
    g_snowman1 = new Snowman(g_direct3d, snowman1_pos);
    g_terrain  = new Terrain(g_direct3d);
}

void on_teardown() {
    delete g_input;
    delete g_camera;
    delete g_crate;
    delete g_skybox;
    delete g_snowman0;
    delete g_snowman1;
    delete g_terrain;

    vertex_free();
    light_free();
}

void on_lost() {
    g_crate->on_lost();
    g_terrain->on_lost();
    g_skybox->on_lost();
    g_snowman0->on_lost();
    g_snowman1->on_lost();
}

void on_reset(int width, int height) {
    g_camera->set_lens(width * 1.0f / height);

    g_crate->on_reset();
    g_terrain->on_reset();
    g_skybox->on_reset();
    g_snowman0->on_reset();
    g_snowman1->on_reset();
}

static void update_objects(float dtime);

void on_render(float dtime) {
    update_objects(dtime);

    D3DXVECTOR3 camera_pos = g_camera->get_pos();
    D3DXMATRIX view_proj = g_camera->get_view_proj();

    OK(g_direct3d->BeginScene());

    g_skybox->render(camera_pos, view_proj);
    g_terrain->render(view_proj);
    g_crate->render(camera_pos, view_proj);
    g_snowman0->render(camera_pos, view_proj);
    g_snowman1->render(camera_pos, view_proj);

    OK(g_direct3d->EndScene());
    g_direct3d->Present(NULL, NULL, NULL, NULL);
}

static void update_objects(float dtime) {
    bool camera_near_crate = g_crate->is_near(g_camera->get_pos());
    bool snowman0_near_crate = g_crate->is_near(g_snowman0->get_pos());
    bool snowman1_near_crate = g_crate->is_near(g_snowman1->get_pos());

    D3DXVECTOR3 crate_offset = g_crate->rotate(g_terrain, dtime);

    if (camera_near_crate) {
        g_camera->set_pos(crate_offset + g_camera->get_pos());
        g_camera->travel(g_input, g_terrain, dtime, g_crate->get_height());
    } else {
        g_camera->travel(g_input, g_terrain, dtime, 0.0f);
    }

    if (snowman0_near_crate) {
        g_snowman0->set_pos(crate_offset + g_snowman0->get_pos());
    }
    if (snowman1_near_crate) {
        g_snowman1->set_pos(crate_offset + g_snowman1->get_pos());
    }
}
