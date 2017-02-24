#include <d3d9.h>
#include <d3dx9.h>
#include "crate.h"
#include "vertex.h"
#include "error.h"

Crate::Crate(IDirect3DDevice9 *direct3d) {
    D3DXMatrixIdentity(&m_world);
    D3DXMatrixTranslation(&m_world, 0, 350, 0);

    build_cube(direct3d);
    build_effect(direct3d);
}

Crate::~Crate() {
    m_effect->Release();
    m_texture->Release();

    m_vertices->Release();
    m_indices->Release();
}

void Crate::on_lost() {
    OK_FX(m_effect->OnLostDevice());
}

void Crate::on_reset() {
    OK_FX(m_effect->OnResetDevice());
}

void Crate::render(IDirect3DDevice9 *direct3d, D3DXVECTOR3 pos,
                   D3DXMATRIX view_proj) {
    OK_FX(m_effect->SetTechnique(m_fx_tech));

    OK_FX(m_effect->SetMatrix(m_fx_world, &m_world));

    D3DXMATRIX world_inv_trans;
    D3DXMatrixInverse(&world_inv_trans, NULL, &m_world);
    D3DXMatrixTranspose(&world_inv_trans, &world_inv_trans);
    OK_FX(m_effect->SetMatrix(m_fx_world_it, &world_inv_trans));

    D3DXMATRIX world_view_proj = m_world * view_proj;
    OK_FX(m_effect->SetMatrix(m_fx_wvp, &world_view_proj));

    OK_3D(direct3d->SetVertexDeclaration(VertexPNT::decl));
    OK_3D(direct3d->SetStreamSource(0, m_vertices, 0, sizeof(VertexPNT)));
    OK_3D(direct3d->SetIndices(m_indices));

    UINT passes = 0;
    OK_FX(m_effect->Begin(&passes, 0));
    OK_FX(m_effect->BeginPass(0));

    OK_3D(direct3d->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12));

    OK_FX(m_effect->EndPass());
    OK_FX(m_effect->End());
}

void Crate::build_cube(IDirect3DDevice9 *direct3d) {
    OK_3D(direct3d->CreateVertexBuffer(24 * sizeof(VertexPNT),
                                       D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED,
                                       &m_vertices, NULL));
    VertexPNT *v;
    OK_3D(m_vertices->Lock(0, 0, (void **)&v, 0));
    // front face
    v[0] = VertexPNT(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[1] = VertexPNT(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[2] = VertexPNT( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    v[3] = VertexPNT( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
    // back face
    v[4] = VertexPNT(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    v[5] = VertexPNT( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    v[6] = VertexPNT( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    v[7] = VertexPNT(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    // top face
    v[8]  = VertexPNT(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    v[9]  = VertexPNT(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    v[10] = VertexPNT( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    v[11] = VertexPNT( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
    // bottom face
    v[12] = VertexPNT(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
    v[13] = VertexPNT( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
    v[14] = VertexPNT( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
    v[15] = VertexPNT(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
    // left face
    v[16] = VertexPNT(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[17] = VertexPNT(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[18] = VertexPNT(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[19] = VertexPNT(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    // right face
    v[20] = VertexPNT( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[21] = VertexPNT( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[22] = VertexPNT( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[23] = VertexPNT( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    OK_3D(m_vertices->Unlock());

    OK_3D(direct3d->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
                                      D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                      &m_indices, 0));
    WORD* i = 0;
    OK_3D(m_indices->Lock(0, 0, (void **)&i, 0));
    // front face
    i[0] = 0; i[1] = 1; i[2] = 2;
    i[3] = 0; i[4] = 2; i[5] = 3;
    // back face
    i[6] = 4; i[7]  = 5; i[8]  = 6;
    i[9] = 4; i[10] = 6; i[11] = 7;
    // top face
    i[12] = 8; i[13] =  9; i[14] = 10;
    i[15] = 8; i[16] = 10; i[17] = 11;
    // bottom face
    i[18] = 12; i[19] = 13; i[20] = 14;
    i[21] = 12; i[22] = 14; i[23] = 15;
    // left face
    i[24] = 16; i[25] = 17; i[26] = 18;
    i[27] = 16; i[28] = 18; i[29] = 19;
    // right face
    i[30] = 20; i[31] = 21; i[32] = 22;
    i[33] = 20; i[34] = 22; i[35] = 23;
    OK_3D(m_indices->Unlock());
}

void Crate::build_effect(IDirect3DDevice9 *direct3d) {
    ID3DXBuffer *errors = NULL;
    OK_3D(D3DXCreateEffectFromFile(direct3d, "crate.fx", NULL, NULL,
                                   D3DXSHADER_DEBUG, NULL, &m_effect, &errors));
    if(errors != NULL) {
        fatal(__FILE__, __LINE__, (const char *)errors->GetBufferPointer());
    }

    m_fx_tech     = m_effect->GetTechniqueByName("DirLightTexTech");
    m_fx_wvp      = m_effect->GetParameterByName(NULL, "gWVP");
    m_fx_world_it = m_effect->GetParameterByName(NULL, "gWorldInvTrans");
    m_fx_world    = m_effect->GetParameterByName(NULL, "gWorld");
    m_fx_eye_pos  = m_effect->GetParameterByName(NULL, "gEyePosW");

    D3DXVECTOR3 mLightVecW     = D3DXVECTOR3(0.0, 0.0f, -1.0f);
    D3DXCOLOR   mAmbientMtrl   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    D3DXCOLOR   mAmbientLight  = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
    D3DXCOLOR   mDiffuseMtrl   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    D3DXCOLOR   mDiffuseLight  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    D3DXCOLOR   mSpecularMtrl  = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
    D3DXCOLOR   mSpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    float       mSpecularPower = 8.0f;

    D3DXHANDLE   mhLightVecW     = m_effect->GetParameterByName(NULL, "gLightVecW");
    D3DXHANDLE   mhDiffuseMtrl   = m_effect->GetParameterByName(NULL, "gDiffuseMtrl");
    D3DXHANDLE   mhDiffuseLight  = m_effect->GetParameterByName(NULL, "gDiffuseLight");
    D3DXHANDLE   mhAmbientMtrl   = m_effect->GetParameterByName(NULL, "gAmbientMtrl");
    D3DXHANDLE   mhAmbientLight  = m_effect->GetParameterByName(NULL, "gAmbientLight");
    D3DXHANDLE   mhSpecularMtrl  = m_effect->GetParameterByName(NULL, "gSpecularMtrl");
    D3DXHANDLE   mhSpecularLight = m_effect->GetParameterByName(NULL, "gSpecularLight");
    D3DXHANDLE   mhSpecularPower = m_effect->GetParameterByName(NULL, "gSpecularPower");

    OK_FX(m_effect->SetValue(mhLightVecW, &mLightVecW, sizeof(D3DXVECTOR3)));
    OK_FX(m_effect->SetValue(mhDiffuseMtrl, &mDiffuseMtrl, sizeof(D3DXCOLOR)));
    OK_FX(m_effect->SetValue(mhDiffuseLight, &mDiffuseLight, sizeof(D3DXCOLOR)));
    OK_FX(m_effect->SetValue(mhAmbientMtrl, &mAmbientMtrl, sizeof(D3DXCOLOR)));
    OK_FX(m_effect->SetValue(mhAmbientLight, &mAmbientLight, sizeof(D3DXCOLOR)));
    OK_FX(m_effect->SetValue(mhSpecularLight, &mSpecularLight, sizeof(D3DXCOLOR)));
    OK_FX(m_effect->SetValue(mhSpecularMtrl, &mSpecularMtrl, sizeof(D3DXCOLOR)));
    OK_FX(m_effect->SetFloat(mhSpecularPower, mSpecularPower));

    OK_3D(D3DXCreateTextureFromFile(direct3d, "crate.jpg", &m_texture));
    D3DXHANDLE mhTex = m_effect->GetParameterByName(NULL, "gTex");
    OK_FX(m_effect->SetTexture(mhTex, m_texture));
}
