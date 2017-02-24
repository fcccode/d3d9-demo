#ifndef CRATE_H
#define CRATE_H

#include <d3d9.h>
#include <d3dx9.h>

class Crate {
public:
    Crate(IDirect3DDevice9 *direct3d);
    ~Crate();

    void on_lost();
    void on_reset();
    void render(IDirect3DDevice9 *direct3d, D3DXVECTOR3 pos,
                D3DXMATRIX view_proj);


private:
    void build_cube(IDirect3DDevice9 *direct3d);
    void build_effect(IDirect3DDevice9 *direct3d);


private:
    D3DXMATRIX m_world;

    IDirect3DVertexBuffer9 *m_vertices;
    IDirect3DIndexBuffer9 *m_indices;

    ID3DXEffect *m_effect;
    IDirect3DTexture9 *m_texture;

    D3DXHANDLE m_fx_tech;
    D3DXHANDLE m_fx_world;
    D3DXHANDLE m_fx_world_it;
    D3DXHANDLE m_fx_wvp;
    D3DXHANDLE m_fx_eye_pos;
};

#endif
