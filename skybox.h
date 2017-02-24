#ifndef SKYBOX_H
#define SKYBOX_H

#include <d3d9.h>
#include <d3dx9.h>

class Skybox {
public:
    Skybox(IDirect3DDevice9 *direct3d);
    ~Skybox();

    void on_lost();
    void on_reset();
    void render(IDirect3DDevice9 *direct3d, D3DXVECTOR3 pos,
                D3DXMATRIX view_proj);

private:
    ID3DXMesh *m_sphere;
    IDirect3DCubeTexture9 *m_envmap;
    ID3DXEffect *m_effect;

    D3DXHANDLE m_fx_tech;
    D3DXHANDLE m_fx_envmap;
    D3DXHANDLE m_fx_mvp;
};

#endif
