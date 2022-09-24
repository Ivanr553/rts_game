#include "physics.h"

#include "../../game_entities/game_entities.h"

Entity *handle_collisions(Entity *entity)
{
    Render_Item *render_item = entity->render_item;

    if (!entity->can_collide || entity->is_fixed_object)
    {
        return entity;
    }

    Array *entities = global.render.entities;
    for (int i = 0; i < entities->len; i++)
    {
        Entity **c_entity_ptr = get_item_from_array(entities, i);
        Entity *c_entity = *c_entity_ptr;

        if (!c_entity || !c_entity->can_collide || entity->id == c_entity->id)
        {
            continue;
        }

        if (c_entity->unit_radius != 0 && entity->unit_radius != 0)
        {
            float collision_distance = did_circles_collide(entity->pos[0], entity->pos[1], entity->unit_radius, c_entity->pos[0], c_entity->pos[1], c_entity->unit_radius);

            if (collision_distance)
            {
                float distance_x = entity->pos[0] - c_entity->pos[0];
                float distance_y = entity->pos[1] - c_entity->pos[1];
                float sum_radii = entity->unit_radius + c_entity->unit_radius;
                float distance = sqrtf(distance_x * distance_x + distance_y * distance_y);
                if (distance == 0)
                {
                    distance = 1;
                }
                float magnitude = sum_radii - distance;

                float d_x = distance_x / (fabs(distance_x) + fabs(distance_y));
                float d_y = distance_y / (fabs(distance_x) + fabs(distance_y));
                float c_d_x = -distance_x / (fabs(distance_x) + fabs(distance_y));
                float c_d_y = -distance_y / (fabs(distance_x) + fabs(distance_y));

                // printf("magnitude: %f | %f %f %f\n", magnitude, d_x, d_y, d_x + d_y);

                if (c_entity->is_fixed_object)
                {
                    entity->pos[0] += magnitude * d_x / 2;
                    entity->pos[1] += magnitude * d_y / 2;
                    entity->render_item->should_update = 1;
                    continue;
                }

                if (entity->should_move_to_pos)
                {
                    if (entity->stop_should_move_count == 0)
                    {
                        entity->stop_should_move_count = 100;
                    }
                    c_entity->pos[0] += magnitude * c_d_x / 50;
                    c_entity->pos[1] += magnitude * c_d_y / 50;
                }
                if (c_entity->should_move_to_pos)
                {
                    if (c_entity->stop_should_move_count == 0)
                    {
                        c_entity->stop_should_move_count = 100;
                    }
                    entity->pos[0] += magnitude * d_x / 50;
                    entity->pos[1] += magnitude * d_y / 50;
                }
                if (!entity->should_move_to_pos && !c_entity->should_move_to_pos)
                {
                    c_entity->pos[0] += magnitude * c_d_x / 50;
                    c_entity->pos[1] += magnitude * c_d_y / 50;
                    entity->pos[0] += magnitude * d_x / 50;
                    entity->pos[1] += magnitude * d_y / 50;
                }

                entity->render_item->should_update = 1;
                c_entity->render_item->should_update = 1;
            }
        }
    }

    return entity;
};