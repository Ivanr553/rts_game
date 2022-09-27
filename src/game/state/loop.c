#include <linmath.h>

#include "../../engine/engine.h"

#include "../../generic/generic.h"

#include "loop.h"
#include "state.h"
#include "../game_entities/game_entities.h"

/** MAIN ENTRY FOR STATE LOOP */
void game_loop(void)
{
    // printf("Starting game loop\n");
    handle_events();

    game_loop_update_entities();
    // printf("Ended game loop\n");
};

short handle_entity_class_updates(Entity *entity)
{
    Game_Entity *game_entity = entity->entity_class;

    if (!game_entity)
    {
        return 0;
    }

    if (game_entity->update_command_queue)
    {
        game_entity->update_command_queue(game_entity);
    }

    if (game_entity->combat_component)
    {
        game_entity->combat_component->update_combat_component(game_entity);

        if (game_entity->marked_for_deletion)
        {
            return 1;
        }
    }

    if (game_entity->harvester_component)
    {
        game_entity->harvester_component->update_harvester_component(game_entity);
    }

    if (game_entity->building_component)
    {
        game_entity->building_component->update_building_component(game_entity);
    }

    if (game_entity->ui_component && game_entity->ui_component->update_ui_entity)
    {
        game_entity->ui_component->update_ui_entity(game_entity);
    }

    if(game_entity->builder_component)
    {
        game_entity->builder_component->update_builder_component(game_entity);
    }

    return 0;
};

/** ENTITY UPDATING */

void game_loop_update_entities(void)
{
    Array *entities = global.render.non_static_entities;

    // printf("Length: %d\n", entities->len);

    for (int i = 0; i < entities->len; i++)
    {
        Entity **entity_ptr_ptr = get_item_from_array(entities, i);
        Entity *entity = *entity_ptr_ptr;

        if (!entity)
        {
            printf("Null entity\n");
            continue;
        }

        // printf("Updating state on entity: %d %p\n", entity->id, entity);

        if (handle_entity_class_updates(entity))
        {
            remove_entity(entity);
            memset(&entity->entity_class, 0, sizeof(Game_Entity));
            printf("Doing the thing\n");
            continue;
        }

        if (!entity)
        {
            printf("Null entity\n");
            continue;
        }

        if (!handle_move_to(entity))
        {
            printf("Unable to execute handle_move_to for entityId: %d\n", entity->id);
        }

        if (!update_entity_pos(entity))
        {
            printf("Unable to execute update_entity_pos for entityId: %d\n", entity->id);
        }

        if (!handle_collisions(entity))
        {
            // printf("Unable to execute handle_collisions for entityId: %d\n", entity->id);
        }
        // printf("Finished collision handling\n");

        if (entity->animation_data && entity->animation_data->current_animation)
        {
            animate_entity(entity);
        }

        if (entity->update_entity)
        {
            entity->update_entity(entity);
        }

        if (entity->entity_class_type == 100)
        {
            // printf("Rendering select entity: %f %f\n", entity->pos[0], entity->pos[1]);
        }

        if (!entity->render_item)
        {
            printf("Entity lacking render item\n");
            continue;
        }

        // printf("Finished updating entity\n");
        if (entity->render_item->should_update)
        {
            update_render_item(entity);
            entity->render_item->should_update = 0;
        }
    }
};

void animate_entity(Entity *entity)
{
    if ((global.render.tick % entity->animation_data->current_animation->frame_rate) == 0)
    {
        entity->animation_data->current_frame = (entity->animation_data->current_frame + 1) % (entity->animation_data->current_animation->total_frames);

        entity->offset[0] = entity->animation_data->current_frame + 1;
        entity->render_item->should_update = 1;
    }
};

Entity *handle_move_to(Entity *entity)
{
    // printf("Should move: %d %f %p\n", entity->should_move_to_pos, entity->move_to_pos[0], entity);
    if (entity->should_move_to_pos == 1)
    {
        float move_to_pos_x = entity->move_to_pos[0];
        float move_to_pos_y = entity->move_to_pos[1];

        float velocity_x = entity->velocity[0];
        float velocity_y = entity->velocity[1];

        float distance_x = move_to_pos_x - entity->pos[0];
        float distance_y = move_to_pos_y - entity->pos[1];

        float x_delta = fabs(entity->move_to_pos[0] - entity->pos[0]);
        float y_delta = fabs(entity->move_to_pos[1] - entity->pos[1]);
        float delta_sum = x_delta + y_delta;

        /** TODO: Assuming max velocity of 0 is the absolute max velocity, should be changed up later */
        float x_mag_ratio = fabs(x_delta / delta_sum);
        float y_mag_ratio = fabs(y_delta / delta_sum);

        velocity_x *= x_mag_ratio;
        velocity_y *= y_mag_ratio;

        if (entity->stop_should_move_count > 0)
        {
            if (delta_sum < 1)
            {
                entity->stop_should_move_count--;

                if (entity->stop_should_move_count == 0)
                {
                    distance_x = 0;
                    distance_y = 0;
                }
            }
        }

        // printf("Still moving to: %f %f\n", distance_x, distance_y);
        if (distance_x == 0 && distance_y == 0)
        {
            entity->should_move_to_pos = 0;

            entity->acceleration[0] = 0;
            entity->acceleration[1] = 0;
            entity->acceleration[2] = 0;

            entity->velocity[0] = 0;
            entity->velocity[1] = 0;
            entity->velocity[2] = 0;

            set_animation(entity, DEFAULT_ANIMATION_IDLE);

            return entity;
        }
    }

    return entity;
};

Entity *update_entity_pos(Entity *entity)
{

    float *acceleration = entity->acceleration;
    float *velocity = entity->velocity;

    float new_velocity_x = 0;
    float new_velocity_y = 0;

    if (entity->should_move_to_pos)
    {
        float x_delta = entity->move_to_pos[0] - entity->pos[0];
        float y_delta = entity->move_to_pos[1] - entity->pos[1];
        float delta_sum = fabs(x_delta) + fabs(y_delta);

        /** TODO: Assuming max velocity of 0 is the absolute max velocity, should be changed up later */
        float x_mag_ratio = x_delta / delta_sum;
        float y_mag_ratio = y_delta / delta_sum;

        new_velocity_x = x_mag_ratio * entity->max_velocity[0];
        new_velocity_y = y_mag_ratio * entity->max_velocity[0];

        /** Checking and handling if we are NEARING the position */
        if (fabs(x_delta) < fabs(new_velocity_x) && ((x_delta / x_delta) * (new_velocity_x / new_velocity_x)) != -1)
        {
            new_velocity_x = x_delta;
        }
        /** Checking and handling if we are AT the position */
        else if (x_delta == 0)
        {
            new_velocity_x = 0;
        }

        /** Checking and handling if we are NEARING the position */
        if (fabs(y_delta) < fabs(new_velocity_y) && ((y_delta / y_delta) * (new_velocity_y / new_velocity_y)) != -1)
        {
            new_velocity_y = y_delta;
        }
        /** Checking and handling if we are AT the position */
        else if (y_delta == 0)
        {
            new_velocity_y = 0;
        }
    }

    if (fabs(entity->max_velocity[0]) >= fabs(new_velocity_x))
    {
        entity->velocity[0] = new_velocity_x;
    }

    if (fabs(entity->max_velocity[1]) >= fabs(new_velocity_y))
    {
        entity->velocity[1] = new_velocity_y;
    }

    if (fabs(entity->velocity[0]) < fabs(entity->max_velocity[0]))
    {
        entity->pos[0] += entity->velocity[0];
    }
    if (fabs(entity->velocity[1]) < fabs(entity->max_velocity[1]))
    {
        entity->pos[1] += entity->velocity[1];
    }

    if (fabs(entity->velocity[0]) > 0 || fabs(entity->velocity[1]) > 0)
    {
        entity->render_item->should_update = 1;
    }

    return entity;
};

float *get_coords(float *ray, vec3 start)
{
    float scalar = (float)((double)-start[2] / (double)ray[2]);

    // printf("Scalar: %f\n", scalar);

    float *end = calloc(3, sizeof(float));

    end[0] = start[0] + (scalar * ray[0]);
    end[1] = start[1] + (scalar * ray[1]);
    end[2] = start[2] + (scalar * ray[2]);

    return end;
}

void test_if_map_entity_was_clicked(void)
{
    float mouse_pos[3];
    get_mouse_pos_on_map(mouse_pos);

    // printf("Ray: %f %f %f\n", ray[0], ray[1], ray[2]);
    // printf("Coords: %f %f %f\n", coords[0], coords[1], coords[2]);

    Array *entities = global.render.entities;
    for (int i = 0; i < entities->len; i++)
    {
        // printf("Starting entity loop\n");
        Entity **c_entity_ptr = get_item_from_array(entities, i);
        Entity *c_entity = *c_entity_ptr;

        // printf("Got entity: %d %p\n", c_entity->id, c_entity);
        Render_Item *c_render_item = c_entity->render_item;
        if (!c_entity->can_collide)
        {
            // printf("Is Static: %d %d\n", c_entity->render_item->is_static_object, i);
            continue;
        }

        switch (c_render_item->type)
        {
        case RENDER_ITEM_QUAD:
        case RENDER_ITEM_VERTICAL_QUAD:
            f32 *c_vertices = c_render_item->positional_vertices;
            // printf("Map Coords: %f %f\n", map_coords[0], map_coords[1]);

            float left_bound = c_vertices[0];
            float right_bound = c_vertices[3];
            float top_bound = c_vertices[7];
            float bottom_bound = c_vertices[1];

            short within_x = 0;
            short within_y = 0;

            if ((mouse_pos[0] >= left_bound) && (mouse_pos[0] <= right_bound))
            {
                // printf("Within x bound\n");
                within_x = 1;
            }

            if ((mouse_pos[1] >= bottom_bound) && (mouse_pos[1] <= top_bound))
            {
                // printf("Within y bound\n");
                within_y = 1;
            }

            if ((within_x + within_y) > 1)
            {
                return;
            }

            // printf("Tested collision for line against entity: %d\n", c_entity->id);
            break;
        }
        // printf("Finished entity processing\n");
    }
}