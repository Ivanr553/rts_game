#pragma once

#include <linmath.h>

typedef struct camera {
    vec3 position;
    vec3 orientation;
    vec3 up;
    mat4x4 camera_uniform;
    mat4x4 view;
    mat4x4 projection;

    int width;
    int height;

    float speed;
    float sensitivity;
} Camera;

#define CAMERA_FOV 45
#define CAMERA_NEAR_PLANE 0.1
#define CAMERA_FAR_PLANE 1000

void init_camera(void);
void camera_matrix(float FOVdeg, float nearPlane, float farPlane);
void move_camera(int forwards, int backwards, int rotateLeft, int rotateRight, int up, int down, int rotate_left, int rotate_right);