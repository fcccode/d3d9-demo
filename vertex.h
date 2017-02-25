#ifndef VERTEX_H
#define VERTEX_H

#include <d3d9.h>
#include <d3dx9.h>

void vertex_init(IDirect3DDevice9 *direct3d);
void vertex_free();

struct Vertex {
    Vertex(float px, float py, float pz,
           float nx, float ny, float nz,
           float tu, float tv) : pos(px, py, pz),
                                 normal(nx, ny, nz),
                                 tex(tu, tv) {}

    D3DXVECTOR3 pos;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 tex;

    static IDirect3DVertexDeclaration9 *decl;
};

#endif
