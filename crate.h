#ifndef CRATE_H
#define CRATE_H

#include <d3d9.h>
#include <d3dx9.h>
#include "effect.h"

class Crate {
public:
    Crate(IDirect3DDevice9 *direct3d);
    ~Crate();
    void on_lost();
    void on_reset();
    void render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj);

private:
    void build_cube();
    void write_vertices();
    void write_indices();

private:
    IDirect3DDevice9 *m_direct3d;

    IDirect3DVertexBuffer9 *m_vertices;
    IDirect3DIndexBuffer9 *m_indices;

    Effect *m_effect;
    IDirect3DTexture9 *m_texture;
    Material m_material;
};

#endif
















