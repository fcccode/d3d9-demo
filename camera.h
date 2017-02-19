#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx9.h>
#include "input.h"
#include "terrain.h"

class Camera {
public:
    Camera(float aspect);
    void reset(float aspect);
    void update(Input *input, Terrain *terrain, float dtime);

private:
    void build_view();
    void build_proj();

private:
    float m_aspect;
    float m_y_fov;
    float m_z_near;
    float m_z_far;
    float m_speed;

    D3DXVECTOR3 m_pos;
    D3DXVECTOR3 m_right;
    D3DXVECTOR3 m_up;
    D3DXVECTOR3 m_look;

    D3DXMATRIX m_view;
    D3DXMATRIX m_proj;
    D3DXMATRIX m_view_proj;
};

#endif
