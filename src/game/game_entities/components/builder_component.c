#include "components.h"

#include "../../../engine/engine.h"
#include "../game_entities_internal.h"
#include "../buildings/building.h"

void update_builder_component(Game_Entity *game_entity)
{
    Component_Builder *builder_component = game_entity->builder_component;

    switch (builder_component->state)
    {
    case BUILDER_STATE_IDLE:
    {
        return;
    }

    case BUILDER_STATE_COMMUTING:
    {

        if (!game_entity->entity->should_move_to_pos)
        {
            move_to(game_entity->entity, builder_component->building_location);
            return;
        }

        Building_Data building_data = get_building_data_by_type(builder_component->building_entity_type);

        float dist = sqrtf(powf(game_entity->entity->pos[0] - game_entity->entity->move_to_pos[0], 2) + powf(game_entity->entity->pos[1] - game_entity->entity->move_to_pos[1], 2));

        if ((dist - building_data.unit_radius) <= 0)
        {
            game_entity->entity->should_move_to_pos = 0;
            builder_component->state = BUILDER_STATE_BUILDING;
        }

        break;
    }

    case BUILDER_STATE_BUILDING:
    {
        builder_component->build_time--;

        if (builder_component->build_time == 0)
        {
            builder_component->state = BUILDER_STATE_IDLE;
            create_building(builder_component->building_location, builder_component->building_entity_type);

            complete_command(game_entity);
        }

        break;
    }

    default:
    {
        return;
    }
    }
}

void add_builder_component(Game_Entity *game_entity, int build_speed)
{
    Component_Builder *builder_component = calloc(1, sizeof(Component_Builder));
    builder_component->build_speed = build_speed;
    builder_component->update_builder_component = update_builder_component;
    builder_component->state = BUILDER_STATE_IDLE;

    game_entity->builder_component = builder_component;
}