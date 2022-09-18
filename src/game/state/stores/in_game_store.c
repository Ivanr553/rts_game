#include "../../../engine/engine.h"

#include "in_game_store.h"
#include "../game_global.h"
#include "../../models/models.h"
#include "../../../generic/generic.h"
#include "../../game_entities/game_entities.h"

void initialize_in_game_store(void)
{
    game_global.game_stores.in_game_store.store_type = STORE_IN_GAME;
    game_global.game_stores.in_game_store.control_groups = calloc(SELECTED_UNIT_MAX_COUNT, sizeof(Control_Group));
    game_global.game_stores.in_game_store.selected_control_group = CONTROL_SLOT_ONE;
    game_global.game_stores.in_game_store.selected_units_by_id = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));

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

        if (entity->entity_class_type == ENTITY_CLASS_UNIT)
        {
            Game_Entity *worker = entity->entity_class;
            worker->is_selected = 0;
        }
    }

    for (int i = 0; i < entity_id_array->len; i++)
    {
        long entity_id = *((long *)get_item_from_array(entity_id_array, i));
        Entity *entity = get_entity_by_id(entity_id);

        if (entity->entity_class_type == ENTITY_CLASS_UNIT)
        {
            Game_Entity *worker = entity->entity_class;
            worker->is_selected = 1;

            create_selection(worker->entity);
        }
    }

    game_global.game_stores.in_game_store.selected_units_by_id = entity_id_array;
};

short handle_right_click_unit_interaction(void)
{
    float mouse_pos[3];
    get_mouse_pos_on_map(mouse_pos);

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

            if (clicked_game_entity->resource_component)
            {
                Array *entity_ids = game_global.game_stores.in_game_store.selected_units_by_id;
                if (entity_ids->len)
                {
                    for (int e = 0; e < entity_ids->len; e++)
                    {
                        long entity_id = *((long *)get_item_from_array(entity_ids, e));
                        Entity *selected_entity = get_entity_by_id(entity_id);
                        Game_Entity *selected_unit_game_entity = selected_entity->entity_class;

                        if (selected_unit_game_entity->harvester_component)
                        {
                            start_harvesting(selected_unit_game_entity, clicked_game_entity);
                        }
                    }
                }
            }

            return 1;
        }
    }

    return 0;
};