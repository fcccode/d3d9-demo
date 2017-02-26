#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "snowman.h"
#include "effect.h"
#include "vertex.h"
#include "error.h"

Snowman::Snowman(IDirect3DDevice9 *direct3d) {
    float radius = 1.0f;
    int slices = 20;
    int stacks = 20;
    const char *texture = "marble.bmp";
    const char *effect = "light.fx";

    m_direct3d = direct3d;
    OK(D3DXCreateSphere(m_direct3d, radius, slices, stacks, &m_sphere, NULL));
    build_tex_coords();
    OK(D3DXCreateTextureFromFile(m_direct3d, texture, &m_texture));
    m_effect = new Effect(m_direct3d, effect);
    m_material.ambient  = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
    m_material.diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_material.specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
    m_material.specular_power = 8.0f;
}

Snowman::~Snowman() {
    delete m_effect;
    m_sphere->Release();
    m_texture->Release();
}

void Snowman::on_lost() {
    OK(m_effect->get_handle()->OnLostDevice());
}

void Snowman::on_reset() {
    OK(m_effect->get_handle()->OnResetDevice());
}

void Snowman::render(D3DXVECTOR3 cam_pos, D3DXMATRIX view_proj) {
    D3DXMATRIX world;
    D3DXMatrixTranslation(&world, 10, 350, 0);

    m_effect->prepare(cam_pos, world, view_proj, m_material, m_texture);
    ID3DXEffect *effect = m_effect->get_handle();

    UINT passes;
    OK(effect->Begin(&passes, 0));
    OK(effect->BeginPass(0));

    OK(m_direct3d->SetRenderState(D3DRS_WRAP0, D3DWRAP_U));
    // OK(effect->CommitChanges());
    OK(m_sphere->DrawSubset(0));
    OK(m_direct3d->SetRenderState(D3DRS_WRAP0, 0));

    OK(effect->EndPass());
    OK(effect->End());
}

void Snowman::build_tex_coords() {
    D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
    UINT num_elems;
    OK(Vertex::decl->GetDeclaration(elems, &num_elems));

    ID3DXMesh *temp;
    OK(m_sphere->CloneMesh(D3DXMESH_SYSTEMMEM, elems, m_direct3d, &temp));
    m_sphere->Release();

    Vertex* vert_buff;
    OK(temp->LockVertexBuffer(0, (LPVOID *)&vert_buff));
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
