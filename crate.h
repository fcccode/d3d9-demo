#ifndef CRATE_H
#define CRATE_H

#include <d3d9.h>
#include <d3dx9.h>
#include "effect.h"
#include "terrain.h"

class Crate {
public:
    Crate(IDirect3DDevice9 *direct3d, D3DXVECTOR3 center);
    ~Crate();
    void on_lost();
    void on_reset();
    void render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj);
    D3DXVECTOR3 rotate(Terrain *terrain, float dtime);
    float get_height();
    bool is_near(D3DXVECTOR3 pos);

private:
    void build_cube();
    void write_vertices();
    void write_indices();

private:
    IDirect3DDevice9 *m_direct3d;

    float m_radius;
    float m_period;
    float m_elapsed;
    float m_scale;

    D3DXVECTOR3 m_center;

    D3DXVECTOR3 m_prev_pos;
    D3DXVECTOR3 m_curr_pos;

    D3DXMATRIX m_world;

    IDirect3DVertexBuffer9 *m_vertices;
    IDirect3DIndexBuffer9 *m_indices;

    Effect *m_effect;
    Material m_material;
    IDirect3DTexture9 *m_texture;
};

#endif
