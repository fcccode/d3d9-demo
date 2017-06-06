#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "snowman.h"
#include "effect.h"
#include "vertex.h"
#include "error.h"

Snowman::Snowman(IDirect3DDevice9 *direct3d, D3DXVECTOR3 pos) {
    float radius = 1.0f;
    int slices = 30;
    int stacks = 30;
    const char *body_tex = "resources/body.jpg";
    const char *head_tex = "resources/head.jpg";

    m_direct3d = direct3d;
    m_pos = pos;

    m_body_scale = 2.0f;
    m_head_scale = 1.5f;
    m_body_bias = 0.0f;
    m_head_bias = 2.5f;

    OK(D3DXCreateSphere(m_direct3d, radius, slices, stacks, &m_sphere, NULL));
    OK(D3DXCreateTextureFromFile(m_direct3d, body_tex, &m_body_tex));
    OK(D3DXCreateTextureFromFile(m_direct3d, head_tex, &m_head_tex));
    build_tex_coords();
    m_effect = new Effect(m_direct3d);
    m_material.ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_material.diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_material.specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
    m_material.specular_power = 8.0f;
}

Snowman::~Snowman() {
    delete m_effect;
    m_sphere->Release();
    m_body_tex->Release();
    m_head_tex->Release();
}

void Snowman::on_lost() {
    OK(m_effect->get_handle()->OnLostDevice());
}

void Snowman::on_reset() {
    OK(m_effect->get_handle()->OnResetDevice());
}

void Snowman::render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj) {
    m_effect->set_technique();

    UINT passes;
    ID3DXEffect *effect = m_effect->get_handle();
    OK(effect->Begin(&passes, 0));
    OK(effect->BeginPass(0));
    OK(m_direct3d->SetRenderState(D3DRS_WRAP0, D3DWRAP_U));

    D3DXMATRIX world;
    D3DXMATRIX operation;

    // draw body
    D3DXMatrixIdentity(&world);
    D3DXMatrixScaling(&operation, m_body_scale, m_body_scale, m_body_scale);
    world *= operation;
    D3DXMatrixTranslation(&operation, m_pos.x, m_pos.y + m_body_bias, m_pos.z);
    world *= operation;
    m_effect->set_states(cam_pos, world, view_proj, m_material, m_body_tex);
    OK(effect->CommitChanges());
    OK(m_sphere->DrawSubset(0));

    // draw head
    D3DXMatrixIdentity(&world);
    D3DXMatrixScaling(&operation, m_head_scale, m_head_scale, m_head_scale);
    world *= operation;
    D3DXMatrixTranslation(&operation, m_pos.x, m_pos.y + m_head_bias, m_pos.z);
    world *= operation;
    m_effect->set_states(cam_pos, world, view_proj, m_material, m_head_tex);
    OK(effect->CommitChanges());
    OK(m_sphere->DrawSubset(0));

    OK(m_direct3d->SetRenderState(D3DRS_WRAP0, 0));
    OK(effect->EndPass());
    OK(effect->End());
}

D3DXVECTOR3 Snowman::get_pos() {
    return m_pos;
}

void Snowman::set_pos(D3DXVECTOR3 pos) {
    m_pos = pos;
}

void Snowman::build_tex_coords() {
    D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
    UINT num_elems;
    OK(Vertex::decl->GetDeclaration(elems, &num_elems));

    ID3DXMesh *temp;
    OK(m_sphere->CloneMesh(D3DXMESH_SYSTEMMEM, elems, m_direct3d, &temp));
    m_sphere->Release();

    Vertex *vert_buff;
    OK(temp->LockVertexBuffer(0, (LPVOID*)&vert_buff));
    int num_verts = temp->GetNumVertices();
    for (int i = 0; i < num_verts; i++) {
        D3DXVECTOR3 pos = vert_buff[i].pos;

        float theta = atan2f(pos.z, pos.x);
        float rho = sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
        float phi = acosf(pos.y / rho);

        float u = theta / (2.0f * D3DX_PI);
        float v = phi / D3DX_PI;

        vert_buff[i].tex.x = u;
        vert_buff[i].tex.y = v;
    }
    OK(temp->UnlockVertexBuffer());

    OK(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
                       elems, m_direct3d, &m_sphere));
    temp->Release();
}
