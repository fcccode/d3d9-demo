#include <assert.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "effect.h"
#include "light.h"
#include "error.h"

Effect::Effect(IDirect3DDevice9 *direct3d) {
    const char *effect = "shaders/light.fx";
    ID3DXBuffer *errors = NULL;
    OK(D3DXCreateEffectFromFile(direct3d, effect, NULL, NULL,
                                D3DXSHADER_DEBUG, NULL, &m_effect, &errors));
    assert(errors == NULL);

    m_fx_tech = m_effect->GetTechniqueByName("LightTech");

    m_fx_eye_pos = m_effect->GetParameterByName(NULL, "g_eye_pos");
    m_fx_world = m_effect->GetParameterByName(NULL, "g_world");
    m_fx_world_it = m_effect->GetParameterByName(NULL, "g_world_it");
    m_fx_wvp = m_effect->GetParameterByName(NULL, "g_wvp");

    m_fx_ambient_light = m_effect->GetParameterByName(NULL, "g_ambient_light");
    m_fx_diffuse_light = m_effect->GetParameterByName(NULL, "g_diffuse_light");
    m_fx_specular_light = m_effect->GetParameterByName(NULL, "g_specular_light");
    m_fx_light_direction = m_effect->GetParameterByName(NULL, "g_light_direction");

    m_fx_ambient_mtrl = m_effect->GetParameterByName(NULL, "g_ambient_mtrl");
    m_fx_diffuse_mtrl = m_effect->GetParameterByName(NULL, "g_diffuse_mtrl");
    m_fx_specular_mtrl = m_effect->GetParameterByName(NULL, "g_specular_mtrl");
    m_fx_specular_power = m_effect->GetParameterByName(NULL, "g_specular_power");

    m_fx_texture = m_effect->GetParameterByName(NULL, "g_texture");
}

Effect::~Effect() {
    m_effect->Release();
}

void Effect::set_technique() {
    OK(m_effect->SetTechnique(m_fx_tech));
}

void Effect::set_states(D3DXVECTOR3 cam_pos,
                        D3DXMATRIX world, D3DXMATRIX view_proj,
                        Material material, IDirect3DTexture9 *texture) {
    int vect3_size = sizeof(D3DXVECTOR3);
    int color_size = sizeof(D3DXCOLOR);
    D3DXMATRIX world_it;
    D3DXMatrixInverse(&world_it, NULL, &world);
    D3DXMatrixTranspose(&world_it, &world_it);
    D3DXMATRIX wvp = world * view_proj;

    OK(m_effect->SetValue(m_fx_eye_pos, &cam_pos, vect3_size));
    OK(m_effect->SetMatrix(m_fx_world, &world));
    OK(m_effect->SetMatrix(m_fx_world_it, &world_it));
    OK(m_effect->SetMatrix(m_fx_wvp, &wvp));

    OK(m_effect->SetValue(m_fx_ambient_light, Light::ambient, color_size));
    OK(m_effect->SetValue(m_fx_diffuse_light, Light::diffuse, color_size));
    OK(m_effect->SetValue(m_fx_specular_light, Light::specular, color_size));
    OK(m_effect->SetValue(m_fx_light_direction, Light::direction, vect3_size));

    OK(m_effect->SetValue(m_fx_ambient_mtrl, &material.ambient, color_size));
    OK(m_effect->SetValue(m_fx_diffuse_mtrl, &material.diffuse, color_size));
    OK(m_effect->SetValue(m_fx_specular_mtrl, &material.specular, color_size));
    OK(m_effect->SetFloat(m_fx_specular_power, material.specular_power));

    OK(m_effect->SetTexture(m_fx_texture, texture));
}

ID3DXEffect *Effect::get_handle() {
    return m_effect;
}
