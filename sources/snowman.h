#ifndef SNOWMAN_H
#define SNOWMAN_H

#include <d3d9.h>
#include <d3dx9.h>
#include "effect.h"

class Snowman {
public:
    Snowman(IDirect3DDevice9 *direct3d, D3DXVECTOR3 pos);
    ~Snowman();
    void on_lost();
    void on_reset();
    void render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj);
    D3DXVECTOR3 get_pos();
    void set_pos(D3DXVECTOR3 pos);

private:
    void build_tex_coords();

private:
    IDirect3DDevice9 *m_direct3d;
    D3DXVECTOR3 m_pos;
    float m_body_scale;
    float m_head_scale;
    float m_body_bias;
    float m_head_bias;
    ID3DXMesh *m_sphere;
    IDirect3DTexture9* m_body_tex;
    IDirect3DTexture9* m_head_tex;
    Effect *m_effect;
    Material m_material;
};

#endif
