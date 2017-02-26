#include <math.h>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include "terrain.h"
#include "light.h"
#include "vertex.h"
#include "error.h"

using std::vector;

Terrain::Terrain(IDirect3DDevice9 *direct3d) {
    const char *himap  = "heightmap.raw";
    const char *effect = "terrain.fx";
    const char *grass  = "grass.dds";
    const char *dirt   = "dirt.dds";
    const char *stone  = "stone.dds";
    const char *blend  = "blend.dds";

    int rows = 257;
    int cols = 257;
    int drow = 10.0f;
    int dcol = 10.0f;
    float scale  = 3.0f;
    float offset = 0.0f;

    m_direct3d = direct3d;

    load_heightmap(himap, rows, cols, scale, offset);
    build_mesh(rows, cols, drow, dcol);
    build_effect(effect);

    OK(D3DXCreateTextureFromFile(m_direct3d, grass, &m_grass));
    OK(D3DXCreateTextureFromFile(m_direct3d, dirt,  &m_dirt));
    OK(D3DXCreateTextureFromFile(m_direct3d, stone, &m_stone));
    OK(D3DXCreateTextureFromFile(m_direct3d, blend, &m_blend));
}

Terrain::~Terrain() {
    m_mesh->Release();
    m_effect->Release();

    m_grass->Release();
    m_dirt->Release();
    m_stone->Release();
    m_blend->Release();
}

void Terrain::on_lost() {
    OK(m_effect->OnLostDevice());
}

void Terrain::on_reset() {
    OK(m_effect->OnResetDevice());
}

void Terrain::render(D3DXMATRIX view_proj) {
    OK(m_effect->SetTechnique(m_fx_tech));
    OK(m_effect->SetMatrix(m_fx_view_proj, &view_proj));
    OK(m_effect->SetValue(m_fx_dir_to_sun, Light::direction,
                          sizeof(D3DXVECTOR3)));

    OK(m_effect->SetTexture(m_fx_grass, m_grass));
    OK(m_effect->SetTexture(m_fx_dirt,  m_dirt));
    OK(m_effect->SetTexture(m_fx_stone, m_stone));
    OK(m_effect->SetTexture(m_fx_blend, m_blend));

    UINT passes;
    OK(m_effect->Begin(&passes, 0));
    OK(m_effect->BeginPass(0));

    OK(m_mesh->DrawSubset(0));

    OK(m_effect->EndPass());
    OK(m_effect->End());
}

float Terrain::get_height(float x, float z) {
    float r = -(z - m_z_offset) / m_dz;
    float c = +(x - m_x_offset) / m_dx;

    int row = (int)floorf(r);
    int col = (int)floorf(c);

    // A*--*B
    //  | /|
    //  |/ |
    // C*--*D
    float A = m_heightmap[row + 0][col + 0];
    float B = m_heightmap[row + 0][col + 1];
    float C = m_heightmap[row + 1][col + 0];
    float D = m_heightmap[row + 1][col + 1];

    float r_diff = r - row;
    float c_diff = c - col;

    if (r_diff + c_diff < 1.0f) {   // upper triangle
        float uy = B - A;
        float vy = C - A;
        return A + c_diff * uy + r_diff * vy;
    } else {                        // lower triangle
        float uy = C - D;
        float vy = B - D;
        return D + (1.0f - c_diff) * uy + (1.0f - r_diff) * vy;
    }
}

void Terrain::load_heightmap(const char *himap, int rows, int cols,
                             float scale, float offset) {
    m_heightmap = vector<vector<float>>(rows, vector<float>(cols));

    int num_elems = rows * cols;
    unsigned char *buffer = new unsigned char[num_elems];
    FILE *file = fopen(himap, "rb");
    if (file == NULL) {
        FATAL("fopen");
    }
    int count = fread(buffer, 1, num_elems, file);
    if (count != num_elems) {
        FATAL("fread");
    }
    fclose(file);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j;
            m_heightmap[i][j] = buffer[index] * scale + offset;
        }
    }
    delete[] buffer;

    smooth_heightmap();
}

void Terrain::smooth_heightmap() {
    vector<vector<float>> copy = m_heightmap;
    int rows = m_heightmap.size();
    int cols = m_heightmap[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float sum = 0.0f;
            int num = 0;
            for (int m = i - 1; m <= i + 1; m++) {
                for (int n = j - 1; n <= j + 1; n++) {
                    if (m >= 0 && m < rows && n >= 0 && n < cols) {
                        sum += copy[m][n];
                        num += 1;
                    }
                }
            }
            m_heightmap[i][j] = sum / num;
        }
    }
}

void Terrain::build_mesh(int rows, int cols, int drow, int dcol) {
    build_params(rows, cols, drow, dcol);

    vector<D3DXVECTOR3> vertices;
    vector<DWORD> indices;
    build_grid(vertices, indices);

    int num_faces = m_cells_z * m_cells_x * 2;
    int num_verts = m_verts_z * m_verts_x;
    D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
    UINT num_elems;
    OK(Vertex::decl->GetDeclaration(elems, &num_elems));
    OK(D3DXCreateMesh(num_faces, num_verts, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
                      elems, m_direct3d, &m_mesh));

    write_vertices(vertices, num_verts);
    write_indices(indices, num_faces);

    OK(D3DXComputeNormals(m_mesh, NULL));

    DWORD *adjacency = new DWORD[num_faces * 3];
    OK(m_mesh->GenerateAdjacency(0.001f, adjacency));
    OK(m_mesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE|D3DXMESHOPT_ATTRSORT,
                               adjacency, NULL, NULL, NULL));
    delete[] adjacency;
}

void Terrain::build_params(int rows, int cols, int drow, int dcol) {
    m_verts_x = cols;
    m_verts_z = rows;
    m_cells_x = m_verts_x - 1;
    m_cells_z = m_verts_z - 1;

    m_dx = dcol;
    m_dz = drow;

    m_width = m_cells_x * m_dx;
    m_depth = m_cells_z * m_dz;

    m_x_offset = -(m_width * 0.5f);
    m_z_offset = +(m_depth * 0.5f);
}

void Terrain::build_grid(vector<D3DXVECTOR3> &vertices,
                         vector<DWORD> &indices) {
    int i = 0;
    vertices.resize(m_verts_z * m_verts_x);
    for (int z = 0; z < m_verts_z; z++) {
        for (int x = 0; x < m_verts_x; x++) {
            vertices[i].x = +x * m_dx + m_x_offset;
            vertices[i].y = 0.0f;
            vertices[i].z = -z * m_dz + m_z_offset;

            i += 1;
        }
    }

    int j = 0;
    indices.resize(m_cells_z * m_cells_x * 6);
    for (int z = 0; z < m_cells_z; z++) {
        for (int x = 0; x < m_cells_x; x++) {
            indices[j + 0] = (z + 0) * m_verts_x + x + 0;
            indices[j + 1] = (z + 0) * m_verts_x + x + 1;
            indices[j + 2] = (z + 1) * m_verts_x + x + 0;

            indices[j + 3] = (z + 1) * m_verts_x + x + 0;
            indices[j + 4] = (z + 0) * m_verts_x + x + 1;
            indices[j + 5] = (z + 1) * m_verts_x + x + 1;

            j += 6;
        }
    }
}

void Terrain::write_vertices(vector<D3DXVECTOR3> &vertices, int num_verts) {
    Vertex *v;
    OK(m_mesh->LockVertexBuffer(0, (LPVOID *)&v));
    for (int k = 0; k < num_verts; k++) {
        int z = k / m_verts_x;
        int x = k % m_verts_x;

        v[k].pos   = vertices[k];
        v[k].pos.y = m_heightmap[z][x];

        v[k].tex.x = +(v[k].pos.x - m_x_offset) / m_width;
        v[k].tex.y = -(v[k].pos.z - m_z_offset) / m_depth;
    }
    OK(m_mesh->UnlockVertexBuffer());
}

void Terrain::write_indices(vector<DWORD> &indices, int num_faces) {
    DWORD *i;
    OK(m_mesh->LockIndexBuffer(0, (LPVOID *)&i));
    DWORD *a;
    OK(m_mesh->LockAttributeBuffer(0, &a));
    for (int k = 0; k < num_faces; k++) {
        i[k * 3 + 0] = indices[k * 3 + 0];
        i[k * 3 + 1] = indices[k * 3 + 1];
        i[k * 3 + 2] = indices[k * 3 + 2];

        a[k] = 0;
    }
    OK(m_mesh->UnlockIndexBuffer());
    OK(m_mesh->UnlockAttributeBuffer());
}

void Terrain::build_effect(const char *effect) {
    ID3DXBuffer *errors = NULL;
    OK(D3DXCreateEffectFromFile(m_direct3d, effect, NULL, NULL,
                                D3DXSHADER_DEBUG, NULL, &m_effect, &errors));
    if (errors != NULL) {
        FATAL((const char *)errors->GetBufferPointer());
    }

    m_fx_tech       = m_effect->GetTechniqueByName("TerrainTech");
    m_fx_view_proj  = m_effect->GetParameterByName(NULL, "gViewProj");
    m_fx_dir_to_sun = m_effect->GetParameterByName(NULL, "gDirToSunW");

    m_fx_grass = m_effect->GetParameterByName(NULL, "gTex0");
    m_fx_dirt  = m_effect->GetParameterByName(NULL, "gTex1");
    m_fx_stone = m_effect->GetParameterByName(NULL, "gTex2");
    m_fx_blend = m_effect->GetParameterByName(NULL, "gBlendMap");
}
