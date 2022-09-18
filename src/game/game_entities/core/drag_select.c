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

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_RECT, SHADER_COLOR, EMPTY_TEXTURE);
    render_item->should_ignore_camera = 1 - SHOW_DRAG_ON_MAP;

    append_item_to_render_item(render_item, entity);
    init_render_item(render_item, pos, size, NULL, (vec4){0, 1, 0, 1});
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

            float *ray_left_top = get_mouse_ray(d_left, d_top);
            float *ray_right_bottom = get_mouse_ray(global.mouse.pos_gl[0], global.mouse.pos_gl[1]);

            float scalar_left_top = (float)((double)-global.camera.position[2] / (double)ray_left_top[2]);
            float scalar_right_bottom = (float)((double)-global.camera.position[2] / (double)ray_right_bottom[2]);

            float coords_left_top[3] = {global.camera.position[0] + (scalar_left_top * ray_left_top[0]), global.camera.position[1] + (scalar_left_top * ray_left_top[1]), global.camera.position[2] + (scalar_left_top * ray_left_top[2])};
            float coords_right_bottom[3] = {global.camera.position[0] + (scalar_right_bottom * ray_right_bottom[0]), global.camera.position[1] + (scalar_right_bottom * ray_right_bottom[1]), global.camera.position[2] + (scalar_right_bottom * ray_right_bottom[2])};

            float left = coords_left_top[0];
            float right = coords_right_bottom[0];
            float top = coords_left_top[1];
            float bottom = coords_right_bottom[1];

            entity->pos[0] = left + (right - left) / 2;
            entity->pos[1] = top + (bottom - top) / 2;

            entity->render_item->size[0] = (right - left);
            entity->render_item->size[1] = (bottom - top);
        }
        else
        {
            float bounds[6];
            get_drag_select_bounds(bounds);

            entity->pos[0] = bounds[0] + (bounds[4] / 2);
            entity->pos[1] = bounds[2] + (bounds[5] / 2);

            entity->render_item->size[0] = bounds[4];
            entity->render_item->size[1] = bounds[5];
        }
    }
    else
    {
        entity->render_item->size[0] = 0;
        entity->render_item->size[1] = 0;
    }

    entity->render_item->should_update = 1;
};