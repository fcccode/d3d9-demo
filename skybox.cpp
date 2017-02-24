#include <d3d9.h>
#include <d3dx9.h>
#include "skybox.h"
#include "error.h"

Skybox::Skybox(IDirect3DDevice9 *direct3d) {
    const char *envmap = "envmap.dds";
    const char *effect = "sky.fx";
    int radius = 10000.0f;
    int slices = 30;
    int stacks = 30;

    OK_3D(D3DXCreateSphere(direct3d, radius, slices, stacks, &m_sphere, NULL));
    OK_3D(D3DXCreateCubeTextureFromFile(direct3d, envmap, &m_envmap));

    ID3DXBuffer *errors = NULL;
    OK_3D(D3DXCreateEffectFromFile(direct3d, effect, NULL, NULL,
                                   D3DXSHADER_DEBUG, NULL, &m_effect, &errors));
    if (errors != NULL) {
        fatal(__FILE__, __LINE__, (const char *)errors->GetBufferPointer());
    }

    m_fx_tech   = m_effect->GetTechniqueByName("SkyTech");
    m_fx_mvp    = m_effect->GetParameterByName(NULL, "gWVP");
    m_fx_envmap = m_effect->GetParameterByName(NULL, "gEnvMap");
}

Skybox::~Skybox() {
    m_sphere->Release();
    m_envmap->Release();
    m_effect->Release();
}

void Skybox::on_lost() {
    OK_FX(m_effect->OnLostDevice());
}

void Skybox::on_reset() {
    OK_FX(m_effect->OnResetDevice());
}

void Skybox::render(IDirect3DDevice9 *direct3d, D3DXVECTOR3 pos,
                    D3DXMATRIX view_proj) {
    OK_FX(m_effect->SetTechnique(m_fx_tech));
    OK_FX(m_effect->SetTexture(m_fx_envmap, m_envmap));

    D3DXMATRIX model;
    D3DXMatrixTranslation(&model, pos.x, pos.y, pos.z);
    D3DXMATRIX mvp = model * view_proj;
    OK_FX(m_effect->SetMatrix(m_fx_mvp, &mvp));

    UINT passes = 0;
    OK_FX(m_effect->Begin(&passes, 0));
    OK_FX(m_effect->BeginPass(0));

    OK_3D(m_sphere->DrawSubset(0));

    OK_FX(m_effect->EndPass());
    OK_FX(m_effect->End());
}
