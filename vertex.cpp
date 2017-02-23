#include <d3d9.h>
#include "vertex.h"
#include "error.h"

IDirect3DVertexDeclaration9 *VertexPNT::decl;

void vertex_decl_init(IDirect3DDevice9 *direct3d) {
    D3DVERTEXELEMENT9 pnt_elems[] = {
        {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
        {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        D3DDECL_END()
    };
    OK_3D(direct3d->CreateVertexDeclaration(pnt_elems, &VertexPNT::decl));
}

void vertex_decl_free() {
    VertexPNT::decl->Release();
}
