#ifndef LIGHT_H
#define LIGHT_H

#include <d3dx9.h>

void light_init();
void light_free();

struct Light {
    static D3DXCOLOR *ambient;
    static D3DXCOLOR *diffuse;
    static D3DXCOLOR *specular;
    static D3DXVECTOR3 *direction;
};

#endif
