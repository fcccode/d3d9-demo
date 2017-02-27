#include <d3dx9.h>
#include "light.h"

D3DXCOLOR *Light::ambient;
D3DXCOLOR *Light::diffuse;
D3DXCOLOR *Light::specular;
D3DXVECTOR3 *Light::direction;

void light_init() {
    Light::ambient  = new D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
    Light::diffuse  = new D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    Light::specular = new D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    Light::direction = new D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void light_free() {
    delete Light::ambient;
    delete Light::diffuse;
    delete Light::specular;
    delete Light::direction;
}
