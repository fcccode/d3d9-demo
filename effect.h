#ifndef EFFECT_H
#define EFFECT_H

#include <d3d9.h>
#include <d3dx9.h>

struct Material {
    D3DXCOLOR ambient;
    D3DXCOLOR diffuse;
    D3DXCOLOR specular;
    float specular_power;
};

class Effect {
public:
    Effect(IDirect3DDevice9 *direct3d, const char *effect);
    ~Effect();
    void set_technique();
    void set_states(D3DXVECTOR3 cam_pos, D3DXMATRIX world, D3DXMATRIX view_proj,
                    Material material, IDirect3DTexture9 *texture);
    ID3DXEffect *get_handle();

private:
    ID3DXEffect *m_effect;

    D3DXHANDLE m_fx_tech;

    D3DXHANDLE m_fx_eye_pos;
    D3DXHANDLE m_fx_world;
    D3DXHANDLE m_fx_world_it;
    D3DXHANDLE m_fx_wvp;

    D3DXHANDLE m_fx_ambient_light;
    D3DXHANDLE m_fx_diffuse_light;
    D3DXHANDLE m_fx_specular_light;
    D3DXHANDLE m_fx_light_direction;

    D3DXHANDLE m_fx_ambient_mtrl;
    D3DXHANDLE m_fx_diffuse_mtrl;
    D3DXHANDLE m_fx_specular_mtrl;
    D3DXHANDLE m_fx_specular_power;

    D3DXHANDLE m_fx_texture;
};

#endif
