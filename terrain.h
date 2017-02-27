#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

class Terrain {
public:
    Terrain(IDirect3DDevice9 *direct3d);
    ~Terrain();
    void on_lost();
    void on_reset();
    void render(D3DXMATRIX view_proj);
    bool is_bounded(float x, float z);
    float get_height(float x, float z);

private:
    void load_heightmap(const char *himap, int rows, int cols,
                        float scale, float offset);
    void smooth_heightmap();

    void build_mesh(int rows, int cols, int drow, int dcol);
    void build_params(int rows, int cols, int drow, int dcol);
    void build_grid(std::vector<D3DXVECTOR3> &vertices,
                    std::vector<DWORD> &indices);
    void write_vertices(std::vector<D3DXVECTOR3> &vertices, int num_verts);
    void write_indices(std::vector<DWORD> &indices, int num_faces);

    void build_effect(const char *effect);

private:
    IDirect3DDevice9 *m_direct3d;

    int m_verts_x;
    int m_verts_z;
    int m_cells_x;
    int m_cells_z;
    float m_dx;
    float m_dz;
    float m_width;
    float m_depth;
    float m_x_offset;
    float m_z_offset;

    std::vector<std::vector<float>> m_heightmap;
    ID3DXMesh *m_mesh;
    ID3DXEffect *m_effect;
    IDirect3DTexture9 *m_ground;

    D3DXHANDLE m_fx_tech;
    D3DXHANDLE m_fx_view_proj;
    D3DXHANDLE m_fx_light_dir;
    D3DXHANDLE m_fx_texture;
};

#endif
