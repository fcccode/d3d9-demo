#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx9.h>
#include "input.h"
#include "terrain.h"

class Camera {
public:
    Camera(float aspect, D3DXVECTOR3 pos);
    void travel(Input *input, Terrain *terrain, float dtime, float y_offset);
    void set_lens(float aspect);
    D3DXMATRIX get_view_proj();
    D3DXVECTOR3 get_pos();
    void set_pos(D3DXVECTOR3 pos);

private:
    void build_view();
    void build_proj(float aspect);

private:
    float m_y_fov;
    float m_z_near;
    float m_z_far;

    float m_mov_speed;
    float m_rot_ratio;
    float m_cam_height;

    D3DXVECTOR3 m_pos;

    D3DXVECTOR3 m_right;
    D3DXVECTOR3 m_up;
    D3DXVECTOR3 m_look;

    D3DXMATRIX m_view;
    D3DXMATRIX m_proj;
    D3DXMATRIX m_view_proj;
};

#endif
