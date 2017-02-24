#include <d3dx9.h>
#include "camera.h"
#include "input.h"
#include "terrain.h"

Camera::Camera(float aspect) {
    m_y_fov  = D3DX_PI * 0.25f;
    m_z_near = 0.01f;
    m_z_far  = 5000.0f;

    m_mov_speed  = 50.0f;
    m_rot_speed  = 150.0f;
    m_cam_height = 2.5f;

    m_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    m_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

    D3DXMatrixIdentity(&m_view);
    D3DXMatrixIdentity(&m_proj);
    D3DXMatrixIdentity(&m_view_proj);

    build_proj(aspect);
    m_view_proj = m_view * m_proj;
}

void Camera::on_reset(float aspect) {
    build_proj(aspect);
    m_view_proj = m_view * m_proj;
}

void Camera::travel(Input *input, Terrain *terrain, float dtime) {
    input->poll();

    D3DXVECTOR3 direction(0.0f, 0.0f, 0.0f);
    if (input->key_down(KEY_W)) {
        direction += m_look;
    }
    if (input->key_down(KEY_S)) {
        direction -= m_look;
    }
    if (input->key_down(KEY_D)) {
        direction += m_right;
    }
    if (input->key_down(KEY_A)) {
        direction -= m_right;
    }

    D3DXVec3Normalize(&direction, &direction);
    D3DXVECTOR3 approx_pos = m_pos + dtime * m_mov_speed * direction;
    float approx_height = terrain->get_height(approx_pos.x, approx_pos.z);
    approx_pos.y = approx_height + m_cam_height;

    D3DXVECTOR3 tangent = approx_pos - m_pos;
    D3DXVec3Normalize(&tangent, &tangent);
    m_pos += dtime * m_mov_speed * tangent;
    m_pos.y = terrain->get_height(m_pos.x, m_pos.z) + m_cam_height;

    float r_angle = input->mouse_dy() / m_rot_speed;
    float y_angle = input->mouse_dx() / m_rot_speed;

    D3DXMATRIX rotation;
    D3DXMatrixRotationAxis(&rotation, &m_right, r_angle);
    D3DXVec3TransformCoord(&m_look, &m_look, &rotation);
    D3DXVec3TransformCoord(&m_up, &m_up, &rotation);

    D3DXMatrixRotationY(&rotation, y_angle);
    D3DXVec3TransformCoord(&m_right, &m_right, &rotation);
    D3DXVec3TransformCoord(&m_up, &m_up, &rotation);
    D3DXVec3TransformCoord(&m_look, &m_look, &rotation);

    build_view();
    m_view_proj = m_view * m_proj;
}

D3DXVECTOR3 Camera::get_pos() {
    return m_pos;
}

D3DXMATRIX Camera::get_view_proj() {
    return m_view_proj;
}

void Camera::build_view() {
    D3DXVec3Normalize(&m_look, &m_look);

    D3DXVec3Cross(&m_up, &m_look, &m_right);
    D3DXVec3Normalize(&m_up, &m_up);

    D3DXVec3Cross(&m_right, &m_up, &m_look);
    D3DXVec3Normalize(&m_right, &m_right);

    float x = -D3DXVec3Dot(&m_pos, &m_right);
    float y = -D3DXVec3Dot(&m_pos, &m_up);
    float z = -D3DXVec3Dot(&m_pos, &m_look);

    m_view(0, 0) = m_right.x;
    m_view(1, 0) = m_right.y;
    m_view(2, 0) = m_right.z;
    m_view(3, 0) = x;

    m_view(0, 1) = m_up.x;
    m_view(1, 1) = m_up.y;
    m_view(2, 1) = m_up.z;
    m_view(3, 1) = y;

    m_view(0, 2) = m_look.x;
    m_view(1, 2) = m_look.y;
    m_view(2, 2) = m_look.z;
    m_view(3, 2) = z;

    m_view(0, 3) = 0.0f;
    m_view(1, 3) = 0.0f;
    m_view(2, 3) = 0.0f;
    m_view(3, 3) = 1.0f;
}

void Camera::build_proj(float aspect) {
    D3DXMatrixPerspectiveFovLH(&m_proj, m_y_fov, aspect, m_z_near, m_z_far);
}
