#ifndef SNOWMAN_H
#define SNOWMAN_H

#include <d3d9.h>
#include <d3dx9.h>
#include "effect.h"

class Snowman {
public:
    Snowman(IDirect3DDevice9 *direct3d);
    ~Snowman();
    void on_lost();
    void on_reset();
    void render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj);

private:
    void build_tex_coords();

private:
    IDirect3DDevice9 *m_direct3d;

    ID3DXMesh *m_sphere;
    IDirect3DTexture9* m_texture;
    Effect *m_effect;
    Material m_material;
};

#endif


