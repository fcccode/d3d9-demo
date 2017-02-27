#include <math.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "crate.h"
#include "effect.h"
#include "terrain.h"
#include "vertex.h"
#include "error.h"

Crate::Crate(IDirect3DDevice9 *direct3d, D3DXVECTOR3 center) {
    const char *effect = "light.fx";
    const char *texture = "crate.jpg";

    m_direct3d = direct3d;

    m_radius = 70.0f;
    m_period = 10.0f;
    m_elapsed = 0.0f;
    m_scale = 3.0f;

    m_center = center;

    D3DXMatrixIdentity(&m_world);

    build_cube();

    m_effect = new Effect(m_direct3d, effect);

    m_material.ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_material.diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_material.specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
    m_material.specular_power = 8.0f;

    OK(D3DXCreateTextureFromFile(m_direct3d, texture, &m_texture));
}

Crate::~Crate() {
    delete m_effect;
    m_texture->Release();
    m_vertices->Release();
    m_indices->Release();
}

void Crate::on_lost() {
    OK(m_effect->get_handle()->OnLostDevice());
}

void Crate::on_reset() {
    OK(m_effect->get_handle()->OnResetDevice());
}

void Crate::render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj) {
    m_effect->set_technique();
    m_effect->set_states(cam_pos, m_world, view_proj, m_material, m_texture);

    OK(m_direct3d->SetVertexDeclaration(Vertex::decl));
    OK(m_direct3d->SetStreamSource(0, m_vertices, 0, sizeof(Vertex)));
    OK(m_direct3d->SetIndices(m_indices));

    UINT passes;
    ID3DXEffect *effect = m_effect->get_handle();
    OK(effect->Begin(&passes, 0));
    OK(effect->BeginPass(0));

    OK(m_direct3d->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12));

    OK(effect->EndPass());
    OK(effect->End());
}

D3DXVECTOR3 Crate::rotate(Terrain *terrain, float dtime) {
    static bool is_initial = true;

    D3DXMATRIX operation;
    D3DXMatrixIdentity(&m_world);

    D3DXMatrixScaling(&operation, m_scale, m_scale, m_scale);
    m_world *= operation;

    D3DXMatrixTranslation(&operation, m_radius, 0.0f, 0.0f);
    m_world *= operation;

    m_elapsed += dtime;
    if (m_elapsed > m_period) {
        m_elapsed -= m_period;
    }
    float radian = m_elapsed / m_period * 2.0f * D3DX_PI;
    D3DXMatrixRotationY(&operation, radian);
    m_world *= operation;

    D3DXMatrixTranslation(&operation, m_center.x, m_center.y, m_center.z);
    m_world *= operation;

    float x = m_world(3, 0);
    float z = m_world(3, 2);
    float y = terrain->get_height(x, z) + m_scale;
    m_world(3, 1) = y;

    m_prev_pos = m_curr_pos;
    m_curr_pos = D3DXVECTOR3(x, y, z);
    if (is_initial) {
        is_initial = false;
        m_prev_pos = m_curr_pos;
    }

    return m_curr_pos - m_prev_pos;
}

float Crate::get_height() {
    return 2.0f * m_scale;
}

bool Crate::is_near(D3DXVECTOR3 pos) {
    D3DXVECTOR3 diff = m_curr_pos - pos;
    if (fabs(diff.x) <= m_scale * 2.0f && fabs(diff.z) <= m_scale * 2.0f) {
        return true;
    } else {
        return false;
    }
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
