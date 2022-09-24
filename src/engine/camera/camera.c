#include "camera.h"

#include "../global.h"
#include "../util/util.h"

void init_camera(void)
{
    mat4x4 camera_uniform;
    Camera camera = {
        .width = viewportWidth,
        .height = viewportHeight,
        .orientation = {0, 0.7, -1},
        .up = {0, 1, 0},
        .speed = 0.08,
        .sensitivity = 5,
        .position = {0, -5, 8}};

    memcpy(&camera.camera_uniform, &camera_uniform, sizeof(camera_uniform));
    global.camera = camera;

    camera_matrix(CAMERA_FOV, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
};

void camera_matrix(float FOVdeg, float nearPlane, float farPlane)
{
    mat4x4 look_at_matrix, projection, view;

    mat4x4_add(&look_at_matrix[0], (vec4 *)global.camera.position, (vec4 *)global.camera.orientation);
    vec3 look_at = {look_at_matrix[0][0], look_at_matrix[0][1], look_at_matrix[0][2]};
    mat4x4_look_at(&view[0], global.camera.position, look_at, global.camera.up);
    mat4x4_perspective(global.camera.projection, FOVdeg, (float)((double)global.camera.width / (double)global.camera.height), nearPlane, farPlane);

    mat4x4_mul(global.camera.camera_uniform, global.camera.projection, &view[0]);
    memcpy(global.camera.view, view, sizeof(mat4x4));
};

void move_camera(int forwards, int backwards, int left, int right, int up, int down, int rotate_left, int rotate_right)
{
    if (forwards)
    {
        global.camera.position[1] += global.camera.speed;
    }
    if (backwards)
    {
        global.camera.position[1] -= global.camera.speed;
    }
    if (left)
    {
        global.camera.position[0] -= global.camera.speed;
    }
    if (right)
    {
        global.camera.position[0] += global.camera.speed;
    }
    if (up)
    {
        global.camera.position[2] += global.camera.speed * 3;
    }
    if (down)
    {
        global.camera.position[2] -= global.camera.speed * 3;
    }
    if (rotate_left)
    {
        global.camera.up[0] -= 0.01;
    }
    if (rotate_right)
    {
        global.camera.up[0] += 0.01;
    }

    // TODO: support camera rotation
    // vec3 cross;
    // cross_vector(cross, global.camera.orientation, global.camera.up);
    // normalize_vector(cross);
    // invert_vector(cross, cross);
    // // add_vector(global.camera.position, )
};