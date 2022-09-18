#include <linmath.h>

#include "mouse_ray.h"

#include "../../../engine/engine.h"

#include "../../state/state.h"

void initialize_mouse_ray(void)
{
    vec3 pos = {0, 0, 0};

    vec4 start;
    float *ray = get_mouse_ray(global.mouse.pos_gl[0], global.mouse.pos_gl[1]);

    float vertices[6] = {global.camera.position[0] + global.mouse.pos_gl[0], global.camera.position[1] + global.mouse.pos_gl[1], global.camera.position[2], -5, 5, -2};
    vec4 color = {255, 255, 255, 255};
    Entity *mouse_ray = create_line_entity(pos, vertices, color);

    if (!append_array(global.render.entities, &mouse_ray))
    {
        return;
    }

    if (!append_array(global.render.non_static_entities, &mouse_ray))
    {
        printf("Adding to array\n");
        return;
    }

    // printf("Mouse Ray is has id: %d\n", mouse_ray->render_item->id);

    game_global.mouse_ray = mouse_ray;
};

float *get_mouse_pos_on_map(float coords[3])
{
    float *ray = get_mouse_ray(global.mouse.pos_gl[0], global.mouse.pos_gl[1]);

    float scalar = (float)((double)-global.camera.position[2] / (double)ray[2]);

    coords[0] = global.camera.position[0] + (scalar * ray[0]);
    coords[1] = global.camera.position[1] + (scalar * ray[1]);
    coords[2] = global.camera.position[2] + (scalar * ray[2]);

    return coords;
}