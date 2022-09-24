#include "in_game_store.h"

#include "../../../engine/engine.h"
#include "../game_global.h"
#include "../../game_entities/game_entities.h"
#include "../events/events.h"

void handle_in_game_event(IN_GAME_EVENT event)
{
    switch (event)
    {
    case IN_GAME_EVENT_ESCAPE:
    {
        if(game_global.game_stores.in_game_store.is_placing_building)
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
    case IN_GAME_EVENT_COMMAND_BUTTON_13:
    case IN_GAME_EVENT_COMMAND_BUTTON_14:
    case IN_GAME_EVENT_COMMAND_BUTTON_15:
    case IN_GAME_EVENT_COMMAND_BUTTON_16:
    case IN_GAME_EVENT_COMMAND_BUTTON_17:
    case IN_GAME_EVENT_COMMAND_BUTTON_18:
    case IN_GAME_EVENT_COMMAND_BUTTON_19:
    case IN_GAME_EVENT_COMMAND_BUTTON_20:
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