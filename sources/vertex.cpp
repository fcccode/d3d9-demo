#include <d3d9.h>
#include "vertex.h"
#include "error.h"

IDirect3DVertexDeclaration9 *Vertex::decl;

void vertex_init(IDirect3DDevice9 *direct3d) {
    D3DVERTEXELEMENT9 elems[] = {
        {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()
    };
    OK(direct3d->CreateVertexDeclaration(elems, &Vertex::decl));
}

void vertex_free() {
    Vertex::decl->Release();
}
