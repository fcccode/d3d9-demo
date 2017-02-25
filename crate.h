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
    void render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj);

private:
    void build_cube();
    void write_vertices();
    void write_indices();
    void build_effect(const char *effect);

private:
    IDirect3DDevice9 *m_direct3d;

    IDirect3DVertexBuffer9 *m_vertices;
    IDirect3DIndexBuffer9 *m_indices;

    D3DXMATRIX m_world;
    ID3DXEffect *m_effect;
    IDirect3DTexture9 *m_texture;

    D3DXVECTOR3 mLightVecW;
    D3DXCOLOR   mAmbientMtrl;
    D3DXCOLOR   mAmbientLight;
    D3DXCOLOR   mDiffuseMtrl;
    D3DXCOLOR   mDiffuseLight;
    D3DXCOLOR   mSpecularMtrl;
    D3DXCOLOR   mSpecularLight;
    float       mSpecularPower;

    D3DXHANDLE mhTech;
    D3DXHANDLE mhWVP;
    D3DXHANDLE mhWorldInvTrans;
    D3DXHANDLE mhLightVecW;
    D3DXHANDLE mhDiffuseMtrl;
    D3DXHANDLE mhDiffuseLight;
    D3DXHANDLE mhAmbientMtrl;
    D3DXHANDLE mhAmbientLight;
    D3DXHANDLE mhSpecularMtrl;
    D3DXHANDLE mhSpecularLight;
    D3DXHANDLE mhSpecularPower;
    D3DXHANDLE mhEyePos;
    D3DXHANDLE mhWorld;
    D3DXHANDLE mhTex;
};

#endif
















