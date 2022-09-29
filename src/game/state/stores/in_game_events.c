#include "in_game_store.h"

#include "../../../engine/engine.h"
#include "../game_global.h"
#include "../../game_entities/game_entities.h"
#include "../events/events.h"

void handle_in_game_event(IN_GAME_EVENT event)
{
    switch (event)
    {

    case IN_GAME_EVENT_DEBUG_DELETE:
    {
        float mouse_pos[3];
        get_mouse_pos_on_map(mouse_pos);

        printf("Attempting to delete unit\n");

        Array *entities = global.render.entities;

        for (int i = 0; i < entities->len; i++)
        {
            Entity **entity_ptr_ptr = get_item_from_array(entities, i);
            Entity *entity = *entity_ptr_ptr;

            if (!entity)
            {
                continue;
            }

            if (is_point_within_circle(entity->pos[0], entity->pos[1], entity->unit_radius, mouse_pos[0], mouse_pos[1]))
            {
                Game_Entity *clicked_game_entity = entity->entity_class;

                if (!clicked_game_entity)
                {
                    continue;
                }

                printf("Delete unit\n");
                remove_entity(entity);
            }
        }
    }

    case IN_GAME_EVENT_ESCAPE:
    {
        if (game_global.game_stores.in_game_store.is_placing_building)
        {
            hide_building_selection();
            return;
        }

        Array *command_button_ids = game_global.game_stores.in_game_store.command_button_ids;
        if (command_button_ids->len)
        {
            show_command_board_buttons_by_selected_unit();
            return;
        }

        break;
    }

    case IN_GAME_EVENT_COMMAND_BUTTON_1:
    case IN_GAME_EVENT_COMMAND_BUTTON_2:
    case IN_GAME_EVENT_COMMAND_BUTTON_3:
    case IN_GAME_EVENT_COMMAND_BUTTON_4:
    case IN_GAME_EVENT_COMMAND_BUTTON_5:
    case IN_GAME_EVENT_COMMAND_BUTTON_6:
    case IN_GAME_EVENT_COMMAND_BUTTON_7:
    case IN_GAME_EVENT_COMMAND_BUTTON_8:
    case IN_GAME_EVENT_COMMAND_BUTTON_9:
    case IN_GAME_EVENT_COMMAND_BUTTON_10:
    case IN_GAME_EVENT_COMMAND_BUTTON_11:
    case IN_GAME_EVENT_COMMAND_BUTTON_12:
    {
        Array *command_button_ids = game_global.game_stores.in_game_store.command_button_ids;

        for (int i = 0; i < command_button_ids->len; i++)
        {
            long id = *((long *)get_item_from_array(command_button_ids, i));
            Entity *entity = get_entity_by_id(id);
            if (!entity)
            {
                continue;
            }

            Game_Entity *game_entity = entity->entity_class;

            printf("Checking for ui type: %d\n", (UI_TYPE_COMMAND_BOARD_BUTTON_1 + (event - IN_GAME_EVENT_COMMAND_BUTTON_1)));
            if (game_entity->ui_component->type == (UI_TYPE_COMMAND_BOARD_BUTTON_1 + (event - IN_GAME_EVENT_COMMAND_BUTTON_1)))
            {
                if (game_entity->ui_component->on_click)
                {
                    game_entity->ui_component->on_click(game_entity);
                }
                return;
            }
        }
        break;
    }
    }
}