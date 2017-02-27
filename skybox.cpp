#include <d3d9.h>
#include <d3dx9.h>
#include "skybox.h"
#include "error.h"

Skybox::Skybox(IDirect3DDevice9 *direct3d) {
    float radius = 10000.0f;
    int slices = 30;
    int stacks = 30;
    const char *effect = "skybox.fx";
    const char *envmap = "envmap.dds";

    m_direct3d = direct3d;
    OK(D3DXCreateSphere(m_direct3d, radius, slices, stacks, &m_sphere, NULL));
    build_effect(effect);
    OK(D3DXCreateCubeTextureFromFile(m_direct3d, envmap, &m_envmap));
}

Skybox::~Skybox() {
    m_sphere->Release();
    m_envmap->Release();
    m_effect->Release();
}

void Skybox::on_lost() {
    OK(m_effect->OnLostDevice());
}

void Skybox::on_reset() {
    OK(m_effect->OnResetDevice());
}

void Skybox::render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj) {
    D3DXMATRIX world;
    D3DXMatrixTranslation(&world, cam_pos.x, cam_pos.y, cam_pos.z);
    D3DXMATRIX wvp = world * view_proj;

    OK(m_effect->SetTechnique(m_fx_tech));
    OK(m_effect->SetMatrix(m_fx_wvp, &wvp));
    OK(m_effect->SetTexture(m_fx_envmap, m_envmap));

    UINT passes = 0;
    OK(m_effect->Begin(&passes, 0));
    OK(m_effect->BeginPass(0));

    OK(m_sphere->DrawSubset(0));

    OK(m_effect->EndPass());
    OK(m_effect->End());
}

void Skybox::build_effect(const char *effect) {
    ID3DXBuffer *errors = NULL;
    OK(D3DXCreateEffectFromFile(m_direct3d, effect, NULL, NULL,
                                D3DXSHADER_DEBUG, NULL, &m_effect, &errors));
    if (errors != NULL) {
        FATAL((const char *)errors->GetBufferPointer());
    }

    m_fx_tech   = m_effect->GetTechniqueByName("SkyTech");
    m_fx_wvp    = m_effect->GetParameterByName(NULL, "g_wvp");
    m_fx_envmap = m_effect->GetParameterByName(NULL, "g_envmap");
}
