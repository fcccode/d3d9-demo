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
    void render(IDirect3DDevice9 *direct3d, D3DXMATRIX view_proj);

    float get_height(float x, float z);

private:
    void load_heightmap(const char *himap, int rows, int cols,
                        float scale, float offset);
    void smooth_heightmap();

    void build_mesh(IDirect3DDevice9 *direct3d,
                    int rows, int cols, int drow, int dcol);
    void build_grid(std::vector<D3DXVECTOR3> &vertices,
                    std::vector<DWORD> &indices);

    void build_effect(IDirect3DDevice9 *direct3d, const char *effect,
                      const char *grass, const char *dirt,
                      const char *stone, const char *blend);

private:
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

    D3DXHANDLE m_fx_tech;
    D3DXHANDLE m_fx_view_proj;

    IDirect3DTexture9 *m_grass;
    IDirect3DTexture9 *m_dirt;
    IDirect3DTexture9 *m_stone;
    IDirect3DTexture9 *m_blend;
};

#endif








