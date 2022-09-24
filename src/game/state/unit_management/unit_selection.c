#include "unit_management.h"

#include "../../../engine/engine.h"
#include "../../../generic/generic.h"

#include "../game_global.h"
#include "../../game_entities/game_entities.h"

void select_units(Array *entity_id_array)
{
    if (!entity_id_array || entity_id_array->len == 0)
    {
        printf("Empty entity_id_array when trying to select units\n");
        return;
    }

    Array *unselected_units = game_global.game_stores.in_game_store.selected_units_by_id;

    for (int i = 0; i < unselected_units->len; i++)
    {
        long entity_id = *((long *)get_item_from_array(unselected_units, i));
        Entity *entity = get_entity_by_id(entity_id);
        if (!entity)
        {
            continue;
        }

        Game_Entity *game_entity = entity->entity_class;

        if (game_entity->selectable_component)
        {
            game_entity->selectable_component->is_selected = 0;
        }
    }

    for (int i = 0; i < entity_id_array->len; i++)
    {
        long entity_id = *((long *)get_item_from_array(entity_id_array, i));
        Entity *entity = get_entity_by_id(entity_id);
        if (!entity)
        {
            continue;
        }
        
        Game_Entity *game_entity = entity->entity_class;

        if (game_entity->selectable_component)
        {
            game_entity->selectable_component->is_selected = 1;

            create_selection(game_entity->entity);
        }
    }

    game_global.game_stores.in_game_store.selected_units_by_id = entity_id_array;
    generate_selection_icons();
    show_command_board_buttons_by_selected_unit();
};