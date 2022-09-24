#include "../../../engine/engine.h"
#include "../../../generic/generic.h"

#include "drag_select.h"

#include "../game_entities_internal.h"

void init_drag_select(void)
{
    /** Initializing data */
    vec2 size = {0, 0};
    vec3 pos = {0, 0, 0};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->update_entity = update_drag_select;
    entity->size[0] = size[0];
    entity->size[1] = size[1];

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_QUAD_WITH_POINTS, SHADER_COLOR, EMPTY_TEXTURE);
    render_item->should_ignore_camera = 1 - SHOW_DRAG_ON_MAP;

    append_item_to_render_item(render_item, entity);
    init_render_item(render_item, pos, entity->size, NULL, (vec4){0, 1, 0, 1}, NULL, NULL);
    bind_render_item_data(render_item);

    add_entity(entity);
}

/** Returns left, right, top, bottom, width, height */
float *get_drag_select_bounds(float bounds[6])
{
    float left = (float)((double)(global.mouse.drag_start_x * 2) / (double)viewportWidth) - 1;
    float top = (float)1 - ((double)(global.mouse.drag_start_y * 2) / (double)viewportHeight);

    float width = global.mouse.pos_gl[0] - left;
    float height = global.mouse.pos_gl[1] - top;

    float bottom = top + height;
    float right = left + width;

    bounds[0] = left;
    bounds[1] = right;
    bounds[2] = top;
    bounds[3] = bottom;
    bounds[4] = width;
    bounds[5] = height;

    return bounds;
}

void update_drag_select(Entity *entity)
{
    if (global.mouse.leftButton == KS_HELD)
    {
        if (SHOW_DRAG_ON_MAP)
        {
            float d_left = (float)((double)(global.mouse.drag_start_x * 2) / (double)viewportWidth) - 1;
            float d_top = (float)1 - ((double)(global.mouse.drag_start_y * 2) / (double)viewportHeight);

            float *ray_left_bottom = get_mouse_ray(d_left, global.mouse.pos_gl[1]);
            float *ray_right_bottom = get_mouse_ray(global.mouse.pos_gl[0], global.mouse.pos_gl[1]);
            float *ray_right_top = get_mouse_ray(global.mouse.pos_gl[0], d_top);
            float *ray_left_top = get_mouse_ray(d_left, d_top);

            float scalar_left_bottom = (float)((double)-global.camera.position[2] / (double)ray_left_bottom[2]);
            float scalar_right_bottom = (float)((double)-global.camera.position[2] / (double)ray_right_bottom[2]);
            float scalar_right_top = (float)((double)-global.camera.position[2] / (double)ray_right_top[2]);
            float scalar_left_top = (float)((double)-global.camera.position[2] / (double)ray_left_top[2]);

            float coords_left_bottom[3] = {global.camera.position[0] + (scalar_left_bottom * ray_left_bottom[0]), global.camera.position[1] + (scalar_left_bottom * ray_left_bottom[1]), global.camera.position[2] + (scalar_left_bottom * ray_left_bottom[2])};
            float coords_right_bottom[3] = {global.camera.position[0] + (scalar_right_bottom * ray_right_bottom[0]), global.camera.position[1] + (scalar_right_bottom * ray_right_bottom[1]), global.camera.position[2] + (scalar_right_bottom * ray_right_bottom[2])};
            float coords_right_top[3] = {global.camera.position[0] + (scalar_right_top * ray_right_top[0]), global.camera.position[1] + (scalar_right_top * ray_right_top[1]), global.camera.position[2] + (scalar_right_top * ray_right_top[2])};
            float coords_left_top[3] = {global.camera.position[0] + (scalar_left_top * ray_left_top[0]), global.camera.position[1] + (scalar_left_top * ray_left_top[1]), global.camera.position[2] + (scalar_left_top * ray_left_top[2])};

            float left = coords_left_top[0];
            float right = coords_right_bottom[0];
            float top = coords_left_top[1];
            float bottom = coords_right_bottom[1];

            entity->pos[0] = left + (right - left) / 2;
            entity->pos[1] = top + (bottom - top) / 2;

            entity->size[0] = (right - left);
            entity->size[1] = (bottom - top);

            entity->points[0] = coords_left_bottom[0];
            entity->points[1] = coords_left_bottom[1];
            entity->points[2] = 0;

            entity->points[3] = coords_right_bottom[0];
            entity->points[4] = coords_right_bottom[1];
            entity->points[5] = 0;

            entity->points[6] = coords_right_top[0];
            entity->points[7] = coords_right_top[1];
            entity->points[8] = 0;

            entity->points[9] = coords_left_top[0];
            entity->points[10] = coords_left_top[1];
            entity->points[11] = 0;
        }
        else
        {
            float bounds[6];
            get_drag_select_bounds(bounds);

            entity->pos[0] = bounds[0] + (bounds[4] / 2);
            entity->pos[1] = bounds[2] + (bounds[5] / 2);

            entity->size[0] = bounds[4];
            entity->size[1] = bounds[5];

            float left = bounds[0];
            float right = bounds[1];
            float top = bounds[2];
            float bottom = bounds[3];
            float width = bounds[4];
            float height = bounds[5];

            entity->points[0] = left;
            entity->points[1] = bottom;
            entity->points[2] = 0;

            entity->points[3] = right;
            entity->points[4] = bottom;
            entity->points[5] = 0;

            entity->points[6] = right;
            entity->points[7] = top;
            entity->points[8] = 0;

            entity->points[9] = left;
            entity->points[10] = top;
            entity->points[11] = 0;
        }
    }
    else
    {
        entity->size[0] = 0;
        entity->size[1] = 0;

        for (int i = 0; i < 12; i++)
        {
            entity->points[i] = 0;
        }
    }

    entity->render_item->updated = 1;
    entity->render_item->should_update = 1;
};