#include "../../../engine/engine.h"

#include "events.h"
#include "../game_global.h"
#include "../../game_entities/game_entities.h"
#include "../../../main.h"

void handle_events(void)
{

    move_camera(
        global.mouse.y == 0 && global.mouse.drag_state != DRAG_STATE_DRAGGING,
        global.mouse.y == global.render.height - 1 && global.mouse.drag_state != DRAG_STATE_DRAGGING,
        global.mouse.x == 0 && global.mouse.drag_state != DRAG_STATE_DRAGGING,
        global.mouse.x == global.render.width - 1 && global.mouse.drag_state != DRAG_STATE_DRAGGING,
        0,
        0,
        0,
        0);

    if (global.input.f10 == KS_PRESSED || global.input.f10 == KS_HELD)
    {
        quit_game();
        return;
    }

    if (global.mouse.rightButton == KS_PRESSED)
    {
        handle_global_event(GLOBAL_EVENT_MOUSE_RIGHT_BUTTON);
    }
    if (global.mouse.drag_state == DRAG_STATE_RELEASED)
    {
        handle_global_event(GLOBAL_EVENT_MOUSE_LEFT_BUTTON_DRAG_RELEASE);
    }
    else if (global.mouse.leftButton == KS_PRESSED)
    {
        game_global.press_count++;

        if (global.mouse.did_double_click)
        {
            printf("Double clicked\n");
        }
    }
    else if (global.mouse.leftButton == KS_HELD)
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

    if (global.input.escape == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_ESCAPE);
    }

    if (global.input.q == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_1);
    }
    if (global.input.w == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_2);
    }
    if (global.input.e == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_3);
    }
    if (global.input.r == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_4);
    }

    if (global.input.a == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_5);
    }
    if (global.input.s == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_6);
    }
    if (global.input.d == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_7);
    }
    if (global.input.f == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_8);
    }

    if (global.input.z == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_9);
    }
    if (global.input.x == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_10);
    }
    if (global.input.c == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_11);
    }
    if (global.input.v == KS_PRESSED)
    {
        handle_in_game_event(IN_GAME_EVENT_COMMAND_BUTTON_12);
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

            float mouse_pos[3];
            get_mouse_pos_on_map(mouse_pos);
            Array *entity_ids = game_global.game_stores.in_game_store.selected_units_by_id;

            if (handle_right_click_unit_interaction(mouse_pos, entity_ids))
            {
                break;
            }

            for (int i = 0; i < entity_ids->len; i++)
            {
                long entity_id = *((long *)get_item_from_array(entity_ids, i));
                Entity *entity = get_entity_by_id(entity_id);

                if (!entity)
                {
                    printf("Unable to move entity, entity was null\n");
                    return;
                }

                Game_Entity *game_entity = entity->entity_class;

                vec3 new_pos = {mouse_pos[0], mouse_pos[1], entity->pos[2]};
                move_to(entity, new_pos);

                if (game_entity->building_component)
                {
                    game_entity->building_component->rally_point[0] = mouse_pos[0];
                    game_entity->building_component->rally_point[1] = mouse_pos[1];
                    game_entity->building_component->rally_point[2] = mouse_pos[2];
                    continue;
                }

                if (game_entity->harvester_component)
                {
                    if (game_entity->harvester_component->state != HARVESTING_STATE_IDLE)
                    {
                        stop_harvesting(game_entity);
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

        Array *selected_units = game_global.game_stores.in_game_store.selected_units_by_id;
        if (game_global.game_stores.in_game_store.is_placing_building)
        {
            Build_Command *build_command = calloc(1, sizeof(Build_Command));
            build_command->pos[0] = mouse_pos[0];
            build_command->pos[1] = mouse_pos[1];
            build_command->pos[2] = 0;
            build_command->total_build_time = 100;
            build_command->type = game_global.game_stores.in_game_store.building_being_placed;

            Queued_Command *command = calloc(1, sizeof(Queued_Command));
            command->queued_command_data = build_command;
            command->type = QUEUED_COMMAND_TYPE_BUILD;

            long entity_id = *((long *)get_item_from_array(selected_units, 0));
            Entity *entity = get_entity_by_id(entity_id);

            if (!entity)
            {
                printf("No entity selected when trying to start build command\n");
                return;
            }
            Game_Entity *game_entity = entity->entity_class;

            command_entity(game_entity, command, 0);

            hide_building_selection();
            return;
        }

        Array *new_selected_units = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));
        Array *entities = global.render.entities;
        for (int i = 0; i < entities->len; i++)
        {
            Entity **entity_ptr = get_item_from_array(entities, i);
            Entity *entity = *entity_ptr;
            Game_Entity *game_entity = entity->entity_class;

            if (!entity || !game_entity)
            {
                continue;
            }

            if (game_entity->selectable_component && !game_entity->resource_component)
            {
                if (is_point_within_circle(entity->pos[0], entity->pos[1], entity->unit_radius, mouse_pos[0], mouse_pos[1]))
                {
                    append_array(new_selected_units, &entity->id);
                    break;
                }
            }
        }

        if (new_selected_units->len)
        {
            select_units(new_selected_units);
        }

        break;
    }
    case GLOBAL_EVENT_MOUSE_LEFT_BUTTON_DRAG_RELEASE:
    {
        printf("Drag released\n");
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

        Array *selected_units = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));

        Array *entities = global.render.entities;
        for (int i = 0; i < entities->len; i++)
        {
            Entity **entity_ptr = get_item_from_array(entities, i);
            Entity *entity = *entity_ptr;
            Game_Entity *game_entity = entity->entity_class;

            if (!entity || !game_entity)
            {
                continue;
            }

            if (game_entity->selectable_component && !game_entity->resource_component)
            {

                short within_x = 0;
                short within_y = 0;

                float *left_point = get_point_on_line(entity->pos[0], entity->pos[1], coords_left_bottom[0], coords_left_bottom[1], coords_left_top[0], coords_left_top[1]);
                float *right_point = get_point_on_line(entity->pos[0], entity->pos[1], coords_right_bottom[0], coords_right_bottom[1], coords_right_top[0], coords_right_top[1]);

                if ((entity->pos[0] >= left_point[0]) && (entity->pos[0] <= right_point[0]))
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