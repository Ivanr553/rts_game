#include "../../../engine/engine.h"

#include "in_game_store.h"
#include "../game_global.h"
#include "../../models/models.h"
#include "../../../generic/generic.h"
#include "../../game_entities/game_entities.h"
#include "../unit_management/unit_management.h"

void initialize_in_game_store(void)
{
    game_global.game_stores.in_game_store.store_type = STORE_IN_GAME;
    game_global.game_stores.in_game_store.control_groups = calloc(SELECTED_UNIT_MAX_COUNT, sizeof(Control_Group));
    game_global.game_stores.in_game_store.selected_control_group = CONTROL_SLOT_ONE;
    game_global.game_stores.in_game_store.selected_units_by_id = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));
    game_global.game_stores.in_game_store.selected_unit_icon_ids = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));
    game_global.game_stores.in_game_store.command_button_ids = create_array(COMMAND_BOARD_BUTTON_COUNT, sizeof(long));
    game_global.game_stores.in_game_store.command_board_info_ids = create_array(COMMAND_BOARD_INFO_COUNT, sizeof(long));

    create_control_groups();
};

void create_control_groups()
{
    for (int i = 0; i < SELECTED_UNIT_MAX_COUNT; i++)
    {
        Array *unit_ids = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));
        Control_Group control_group = {
            .slot = i,
            .unit_ids = unit_ids};

        game_global.game_stores.in_game_store.control_groups[i] = control_group;
    }
}

void control_group_add_unit(Control_Group_Slot slot, long id)
{
    append_array(game_global.game_stores.in_game_store.control_groups[slot].unit_ids, &id);
};

short handle_right_click_unit_interaction(vec3 pos_on_map, Array *entity_ids)
{
    Array *entities = global.render.entities;

    for (int i = 0; i < entities->len; i++)
    {
        Entity **entity_ptr_ptr = get_item_from_array(entities, i);
        Entity *entity = *entity_ptr_ptr;

        if (!entity)
        {
            continue;
        }

        if (is_point_within_circle(entity->pos[0], entity->pos[1], entity->unit_radius, pos_on_map[0], pos_on_map[1]))
        {
            Game_Entity *clicked_game_entity = entity->entity_class;

            if (!clicked_game_entity)
            {
                continue;
            }

            if (clicked_game_entity->player_slot != game_global.game_stores.in_game_store.player->player_slot)
            {
                if (entity_ids->len)
                {
                    for (int e = 0; e < entity_ids->len; e++)
                    {
                        long entity_id = *((long *)get_item_from_array(entity_ids, e));
                        Entity *selected_entity = get_entity_by_id(entity_id);
                        if (!selected_entity)
                        {
                            continue;
                        }
                        Game_Entity *selected_unit_game_entity = selected_entity->entity_class;

                        if (selected_unit_game_entity->combat_component)
                        {
                            selected_unit_game_entity->combat_component->target_entity = clicked_game_entity;
                            selected_unit_game_entity->combat_component->is_attacking = 1;
                        }
                    }
                }
                return 1;
            }

            if (clicked_game_entity->resource_component)
            {
                if (entity_ids->len)
                {
                    for (int e = 0; e < entity_ids->len; e++)
                    {
                        long entity_id = *((long *)get_item_from_array(entity_ids, e));
                        Entity *selected_entity = get_entity_by_id(entity_id);
                        if (!selected_entity)
                        {
                            continue;
                        }
                        Game_Entity *selected_unit_game_entity = selected_entity->entity_class;

                        if (selected_unit_game_entity->harvester_component)
                        {
                            start_harvesting(selected_unit_game_entity, clicked_game_entity);
                        }

                        if (selected_unit_game_entity->building_component)
                        {
                            selected_unit_game_entity->building_component->rally_point[0] = pos_on_map[0];
                            selected_unit_game_entity->building_component->rally_point[1] = pos_on_map[1];
                            selected_unit_game_entity->building_component->rally_point[2] = pos_on_map[2];
                            continue;
                        }
                    }
                }
            }

            return 1;
        }
    }

    return 0;
};