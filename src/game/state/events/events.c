#include "../../../engine/engine.h"

#include "events.h"
#include "../game_global.h"
#include "../../game_entities/game_entities.h"

void handle_events(void)
{
    if (global.mouse.rightButton == KS_PRESSED)
    {
        handle_global_event(GLOBAL_EVENT_MOUSE_RIGHT_BUTTON);
    }
    if (global.mouse.drag_state == DRAG_STATE_RELEASED)
    {
        handle_global_event(GLOBAL_EVENT_MOUSE_LEFT_BUTTON_DRAG_RELEASE);
    }
    else if (global.mouse.leftButton == KS_PRESSED || global.mouse.leftButton == KS_HELD)
    {
        game_global.press_count++;
    }
    else if (global.mouse.leftButton == KS_UNPRESSED && game_global.press_count < 20 && game_global.press_count != 0)
    {
        game_global.press_count = 0;
        handle_global_event(GLOBAL_EVENT_MOUSE_LEFT_BUTTON);
    }
    else
    {
        game_global.press_count = 0;
    }

    if (global.input.b == KS_PRESSED)
    {
        if (game_global.game_stores.in_game_store.is_placing_building)
        {
            game_global.game_stores.in_game_store.is_placing_building = 0;
            hide_building_selection();
        }
        else
        {
            game_global.game_stores.in_game_store.is_placing_building = 1;
            show_building_selection(6, 5);
        }
    }

    if (global.input.one == KS_PRESSED)
    {
        if (global.input.ctrl == KS_PRESSED || global.input.ctrl == KS_HELD)
        {
            if (game_global.game_stores.in_game_store.selected_units_by_id->len > 0)
            {
                game_global.game_stores.in_game_store.control_groups[CONTROL_SLOT_ONE].unit_ids = game_global.game_stores.in_game_store.selected_units_by_id;
            }
        }
        else
        {
            game_global.game_stores.in_game_store.selected_control_group = CONTROL_SLOT_ONE;
            select_units(game_global.game_stores.in_game_store.control_groups[CONTROL_SLOT_ONE].unit_ids);
        }
    }
    if (global.input.two == KS_PRESSED)
    {
        if (global.input.ctrl == KS_PRESSED || global.input.ctrl == KS_HELD)
        {
            if (game_global.game_stores.in_game_store.selected_units_by_id->len > 0)
            {
                game_global.game_stores.in_game_store.control_groups[CONTROL_SLOT_TWO].unit_ids = game_global.game_stores.in_game_store.selected_units_by_id;
            }
        }
        else
        {
            game_global.game_stores.in_game_store.selected_control_group = CONTROL_SLOT_TWO;
            select_units(game_global.game_stores.in_game_store.control_groups[CONTROL_SLOT_TWO].unit_ids);
        }
    }
};

void handle_global_event(Global_Event event)
{
    switch (event)
    {
    case GLOBAL_EVENT_MOUSE_RIGHT_BUTTON:
    {
        if (game_global.state == IN_GAME)
        {
            if (handle_right_click_unit_interaction())
            {
                break;
            }

            Array *entity_ids = game_global.game_stores.in_game_store.selected_units_by_id;

            for (int i = 0; i < entity_ids->len; i++)
            {
                long entity_id = *((long *)get_item_from_array(entity_ids, i));
                Entity *entity = get_entity_by_id(entity_id);

                if (!entity)
                {
                    printf("Unable to move entity, entity was null\n");
                    return;
                }

                float mouse_pos[3];
                get_mouse_pos_on_map(mouse_pos);
                vec3 new_pos = {mouse_pos[0], mouse_pos[1], entity->pos[2]};

                move_to(entity, new_pos);

                if (entity->entity_class_type == ENTITY_CLASS_UNIT)
                {
                    Game_Entity *unit_entity = entity->entity_class;

                    if (unit_entity->harvester_component)
                    {
                        if (unit_entity->harvester_component->state != HARVESTING_STATE_IDLE)
                        {
                            stop_harvesting(unit_entity);
                        }
                    }
                }

                set_animation(entity, DEFAULT_ANIMATION_WALKING);
            }
        }

        break;
    }
    case GLOBAL_EVENT_MOUSE_LEFT_BUTTON:
    {
        if (handle_ui_element_click(global.mouse.pos_gl[0], global.mouse.pos_gl[1]))
        {
            return;
        }

        float mouse_pos[3];
        get_mouse_pos_on_map(mouse_pos);

        Array *selected_units = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));

        Array *entities = global.render.entities;
        for (int i = 0; i < entities->len; i++)
        {
            Entity **entity_ptr = get_item_from_array(entities, i);
            Entity *entity = *entity_ptr;

            if (!entity)
            {
                continue;
            }

            if (entity->entity_class_type == ENTITY_CLASS_UNIT)
            {
                if (is_point_within_circle(entity->pos[0], entity->pos[1], entity->unit_radius, mouse_pos[0], mouse_pos[1]))
                {
                    append_array(selected_units, &entity->id);
                    break;
                }
            }
        }

        if (selected_units->len)
        {
            select_units(selected_units);
        }

        break;
    }
    case GLOBAL_EVENT_MOUSE_LEFT_BUTTON_DRAG_RELEASE:
    {
        printf("Drag released\n");
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

        Array *selected_units = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));

        Array *entities = global.render.entities;
        for (int i = 0; i < entities->len; i++)
        {
            Entity **entity_ptr = get_item_from_array(entities, i);
            Entity *entity = *entity_ptr;

            if (!entity)
            {
                continue;
            }

            if (entity->entity_class_type == ENTITY_CLASS_UNIT)
            {

                short within_x = 0;
                short within_y = 0;

                if ((entity->pos[0] >= left) && (entity->pos[0] <= right))
                {
                    // printf("Within x bound\n");
                    within_x = 1;
                }

                if ((entity->pos[1] >= bottom) && (entity->pos[1] <= top))
                {
                    // printf("Within y bound\n");
                    within_y = 1;
                }

                if ((within_x + within_y) > 1)
                {
                    if (selected_units->len == SELECTED_UNIT_MAX_COUNT)
                    {
                        break;
                    }

                    append_array(selected_units, &entity->id);
                }
            }
        }

        if (selected_units->len)
        {
            select_units(selected_units);
        }

        break;
    }

    default:
        printf("Received invalid global_event of: %d\n", event);
    }
};