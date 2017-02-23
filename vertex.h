#ifndef VERTEX_H
#define VERTEX_H

#include <d3d9.h>
#include <d3dx9.h>

void vertex_decl_init(IDirect3DDevice9 *direct3d);
void vertex_decl_free();

struct VertexPNT {
    D3DXVECTOR3 pos;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 tex;

    static IDirect3DVertexDeclaration9 *decl;
};

#endif
