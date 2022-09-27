#include <linmath.h>

#include "mouse_ray.h"

#include "../../../engine/engine.h"

#include "../../state/state.h"

float *get_mouse_pos_on_map(float coords[3])
{
    float *ray = get_mouse_ray(global.mouse.pos_gl[0], global.mouse.pos_gl[1]);

    float scalar = (float)((double)-global.camera.position[2] / (double)ray[2]);

    coords[0] = global.camera.position[0] + (scalar * ray[0]);
    coords[1] = global.camera.position[1] + (scalar * ray[1]);
    coords[2] = global.camera.position[2] + (scalar * ray[2]);

    return coords;
}