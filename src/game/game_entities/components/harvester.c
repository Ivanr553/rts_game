#include "components.h"

#include "../../../engine/engine.h"

#include "../game_entities_internal.h"
#include "../resource.h"
#include "../../state/state.h"

void add_harvester_component(Game_Entity *unit_entity, int harvesting_speed, float harvesting_distance)
{
    Component_Harvest *harvester_component = calloc(1, sizeof(Component_Harvest));
    harvester_component->harvesting_speed = harvesting_speed;
    harvester_component->state = HARVESTING_STATE_IDLE;
    harvester_component->harvesting_distance = harvesting_distance;

    unit_entity->harvester_component = harvester_component;
};

void add_resource_harvesting_data(Component_Harvest *harvester_component, RESOURCE resource)
{
    switch (resource)
    {
    case RESOURCE_CRYSTAL:
        harvester_component->max_resource_quantity = 10;
        break;

    default:
        printf("Resource type of %d not supported\n", resource);
    }
};

void start_harvesting(Game_Entity *unit_entity, Game_Entity *resource_entity)
{
    printf("Starting harvesting\n");
    unit_entity->entity->should_move_to_pos = 0;
    unit_entity->harvester_component->state = HARVESTING_STATE_COMMUTING;
    add_resource_harvesting_data(unit_entity->harvester_component, resource_entity->resource_component->resource_type);
    unit_entity->harvester_component->resource_entity = resource_entity;

    unit_entity->entity->can_collide = 0;
};

void stop_harvesting(Game_Entity *unit_entity)
{
    printf("Stopping harvesting\n");
    unit_entity->harvester_component->state = HARVESTING_STATE_IDLE;
    unit_entity->harvester_component->max_resource_quantity = 0;
    unit_entity->harvester_component->resource_entity = NULL;

    unit_entity->entity->can_collide = 1;
};

void handle_harvesting(Game_Entity *unit_entity)
{
    Component_Harvest *harvester_component = unit_entity->harvester_component;
    switch (harvester_component->state)
    {
    case HARVESTING_STATE_IDLE:
        break;

    case HARVESTING_STATE_COMMUTING:
        // printf("Commuting: %f\n", harvester_component->harvesting_distance);
        if (!unit_entity->entity->should_move_to_pos)
        {
            move_to(unit_entity->entity, harvester_component->resource_entity->entity->pos);
        }
        else
        {
            float dist = sqrtf(powf(unit_entity->entity->pos[0] - unit_entity->entity->move_to_pos[0], 2) + powf(unit_entity->entity->pos[1] - unit_entity->entity->move_to_pos[1], 2));
            // printf("Dist: %f\n", dist - harvester_component->resource_entity->entity->unit_radius);

            if ((dist - harvester_component->resource_entity->entity->unit_radius) <= harvester_component->harvesting_distance)
            {
                unit_entity->entity->should_move_to_pos = 0;
                harvester_component->state = HARVESTING_STATE_HARVESTING;
            }
        }

        break;

    case HARVESTING_STATE_HARVESTING:
        // printf("Harvesting\n");
        if (harvester_component->harvesting_time_left == 0)
        {
            harvester_component->harvesting_time_left = harvester_component->harvesting_speed;
        }
        else
        {
            harvester_component->harvesting_time_left--;

            if (harvester_component->harvesting_time_left == 0)
            {
                harvester_component->resource_quantity = 10;
                harvester_component->state = HARVESTING_STATE_DEPOSITING;
            }
        }
        break;

    case HARVESTING_STATE_DEPOSITING:
        // printf("Depositing\n");
        if (!unit_entity->entity->should_move_to_pos)
        {
            Array *entities = global.render.entities;

            for (int i = 0; i < entities->len; i++)
            {
                Entity **entity_ptr = get_item_from_array(entities, i);
                Entity *entity = *entity_ptr;

                if (!entity)
                {
                    continue;
                }

                Game_Entity *entity_class = entity->entity_class;

                if (entity_class && entity_class->building_component && entity_class->building_component->building_type == BUILDING_TYPE_BASE)
                {
                    harvester_component->return_building_entity = entity_class;
                    move_to(unit_entity->entity, entity->pos);
                    break;
                }
            }
        }
        else
        {
            float dist = sqrtf(powf(unit_entity->entity->pos[0] - unit_entity->entity->move_to_pos[0], 2) + powf(unit_entity->entity->pos[1] - unit_entity->entity->move_to_pos[1], 2));
            // printf("Dist: %f\n", dist - harvester_component->return_building_entity->entity->unit_radius);

            if ((dist - harvester_component->return_building_entity->entity->unit_radius) <= harvester_component->harvesting_distance)
            {

                game_global.game_stores.in_game_store.player->crystal_count += harvester_component->resource_quantity;
                printf("Deposited resources, new amount: %d\n", game_global.game_stores.in_game_store.player->crystal_count);

                unit_entity->entity->should_move_to_pos = 0;
                harvester_component->resource_quantity = 0;
                harvester_component->state = HARVESTING_STATE_COMMUTING;
            }
        }

        break;

    default:
        printf("Invalid harvesting state: %d\n", harvester_component->state);
        break;
    }
};