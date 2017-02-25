#include <d3d9.h>
#include <d3dx9.h>
#include "crate.h"
#include "vertex.h"
#include "error.h"

Crate::Crate(IDirect3DDevice9 *direct3d) {
    const char *effect = "object.fx";
    const char *texture = "crate.jpg";

    mLightVecW     = D3DXVECTOR3(0.0, 0.0f, -1.0f);
    mDiffuseMtrl   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    mDiffuseLight  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    mAmbientMtrl   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    mAmbientLight  = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
    mSpecularMtrl  = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
    mSpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    mSpecularPower = 8.0f;

    m_direct3d = direct3d;

    D3DXMatrixIdentity(&m_world);
    D3DXMatrixTranslation(&m_world, 0, 350, 0);

    build_cube();
    build_effect(effect);

    OK(D3DXCreateTextureFromFile(m_direct3d, texture, &m_texture));
}

Crate::~Crate() {
    m_effect->Release();
    m_texture->Release();

    m_vertices->Release();
    m_indices->Release();
}

void Crate::on_lost() {
    OK(m_effect->OnLostDevice());
}

void Crate::on_reset() {
    OK(m_effect->OnResetDevice());
}

void Crate::render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj) {
    OK(m_effect->SetTechnique(mhTech));
    OK(m_effect->SetMatrix(mhWVP, &(m_world * view_proj)));

    D3DXMATRIX worldInvTrans;
    D3DXMatrixInverse(&worldInvTrans, NULL, &m_world);
    D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);

    OK(m_effect->SetMatrix(mhWorldInvTrans, &worldInvTrans));
    OK(m_effect->SetValue(mhLightVecW, &mLightVecW, sizeof(D3DXVECTOR3)));
    OK(m_effect->SetValue(mhDiffuseMtrl, &mDiffuseMtrl, sizeof(D3DXCOLOR)));
    OK(m_effect->SetValue(mhDiffuseLight, &mDiffuseLight, sizeof(D3DXCOLOR)));
    OK(m_effect->SetValue(mhAmbientMtrl, &mAmbientMtrl, sizeof(D3DXCOLOR)));
    OK(m_effect->SetValue(mhAmbientLight, &mAmbientLight, sizeof(D3DXCOLOR)));
    OK(m_effect->SetValue(mhSpecularLight, &mSpecularLight, sizeof(D3DXCOLOR)));
    OK(m_effect->SetValue(mhSpecularMtrl, &mSpecularMtrl, sizeof(D3DXCOLOR)));
    OK(m_effect->SetFloat(mhSpecularPower, mSpecularPower));

    OK(m_effect->SetValue(mhEyePos, &cam_pos, sizeof(D3DXVECTOR3)));
    OK(m_effect->SetMatrix(mhWorld, &m_world));
    OK(m_effect->SetTexture(mhTex, m_texture));

    OK(m_direct3d->SetVertexDeclaration(Vertex::decl));
    OK(m_direct3d->SetStreamSource(0, m_vertices, 0, sizeof(Vertex)));
    OK(m_direct3d->SetIndices(m_indices));

    UINT passes;
    OK(m_effect->Begin(&passes, 0));
    OK(m_effect->BeginPass(0));

    OK(m_direct3d->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12));

    OK(m_effect->EndPass());
    OK(m_effect->End());
}

void Crate::build_cube() {
    write_vertices();
    write_indices();
}

void Crate::write_vertices() {
    OK(m_direct3d->CreateVertexBuffer(24 * sizeof(Vertex), D3DUSAGE_WRITEONLY,
                                      0, D3DPOOL_MANAGED,
                                      &m_vertices, NULL));
    Vertex *v;
    OK(m_vertices->Lock(0, 0, (void **)&v, 0));
    // front face
    v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
    v[1] = Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    v[2] = Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    v[3] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
    // back face
    v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    v[5] = Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    v[6] = Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    v[7] = Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    // top face
    v[8]  = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    v[9]  = Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    v[10] = Vertex( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    v[11] = Vertex( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
    // bottom face
    v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
    v[13] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
    v[14] = Vertex( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
    v[15] = Vertex(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
    // left face
    v[16] = Vertex(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[17] = Vertex(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[18] = Vertex(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    // right face
    v[20] = Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    v[21] = Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    v[22] = Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    v[23] = Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    OK(m_vertices->Unlock());
}

void Crate::write_indices() {
    OK(m_direct3d->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
                                     D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                     &m_indices, 0));
    WORD *i;
    OK(m_indices->Lock(0, 0, (void **)&i, 0));
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
    OK(m_indices->Unlock());
}

void Crate::build_effect(const char *effect) {
    ID3DXBuffer *errors = NULL;
    OK(D3DXCreateEffectFromFile(m_direct3d, effect, NULL, NULL,
                                D3DXSHADER_DEBUG, NULL, &m_effect, &errors));
    if(errors != NULL) {
        FATAL((const char *)errors->GetBufferPointer());
    }

    mhTech          = m_effect->GetTechniqueByName("DirLightTexTech");
    mhWVP           = m_effect->GetParameterByName(NULL, "gWVP");
    mhWorldInvTrans = m_effect->GetParameterByName(NULL, "gWorldInvTrans");
    mhLightVecW     = m_effect->GetParameterByName(NULL, "gLightVecW");
    mhDiffuseMtrl   = m_effect->GetParameterByName(NULL, "gDiffuseMtrl");
    mhDiffuseLight  = m_effect->GetParameterByName(NULL, "gDiffuseLight");
    mhAmbientMtrl   = m_effect->GetParameterByName(NULL, "gAmbientMtrl");
    mhAmbientLight  = m_effect->GetParameterByName(NULL, "gAmbientLight");
    mhSpecularMtrl  = m_effect->GetParameterByName(NULL, "gSpecularMtrl");
    mhSpecularLight = m_effect->GetParameterByName(NULL, "gSpecularLight");
    mhSpecularPower = m_effect->GetParameterByName(NULL, "gSpecularPower");
    mhEyePos        = m_effect->GetParameterByName(NULL, "gEyePosW");
    mhWorld         = m_effect->GetParameterByName(NULL, "gWorld");
    mhTex           = m_effect->GetParameterByName(NULL, "gTex");
}
