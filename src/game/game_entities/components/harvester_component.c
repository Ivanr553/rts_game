#include "components.h"

#include "../../../engine/engine.h"

#include "../game_entities_internal.h"
#include "../../state/state.h"

/** Harvesting speed => larger is slower (Will probably want to fix) */
void add_harvester_component(Game_Entity *game_entity, int harvesting_speed, float harvesting_distance)
{
    Component_Harvest *harvester_component = calloc(1, sizeof(Component_Harvest));
    harvester_component->harvesting_speed = harvesting_speed;
    harvester_component->state = HARVESTING_STATE_IDLE;
    harvester_component->harvesting_distance = harvesting_distance;
    harvester_component->update_harvester_component = update_harvester_component;

    game_entity->harvester_component = harvester_component;
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

void start_harvesting(Game_Entity *game_entity, Game_Entity *resource_entity)
{
    printf("Starting harvesting\n");
    game_entity->entity->should_move_to_pos = 0;
    game_entity->harvester_component->state = HARVESTING_STATE_COMMUTING;
    add_resource_harvesting_data(game_entity->harvester_component, resource_entity->resource_component->resource_type);
    game_entity->harvester_component->resource_entity = resource_entity;
    game_entity->harvester_component->return_building_entity = NULL;

    game_entity->entity->can_collide = 0;
};

void stop_harvesting(Game_Entity *game_entity)
{
    printf("Stopping harvesting\n");
    game_entity->harvester_component->state = HARVESTING_STATE_IDLE;
    game_entity->harvester_component->max_resource_quantity = 0;
    game_entity->harvester_component->resource_entity = NULL;

    game_entity->entity->can_collide = 1;
};

void update_harvester_component(Game_Entity *game_entity)
{
    Component_Harvest *harvester_component = game_entity->harvester_component;
    switch (harvester_component->state)
    {
    case HARVESTING_STATE_IDLE:
        break;

    case HARVESTING_STATE_COMMUTING:
        // printf("Commuting: %f\n", harvester_component->harvesting_distance);
        if (!game_entity->entity->should_move_to_pos)
        {
            move_to(game_entity->entity, harvester_component->resource_entity->entity->pos);
        }
        else
        {
            float dist = sqrtf(powf(game_entity->entity->pos[0] - game_entity->entity->move_to_pos[0], 2) + powf(game_entity->entity->pos[1] - game_entity->entity->move_to_pos[1], 2));
            // printf("Dist: %f\n", dist - harvester_component->resource_entity->entity->unit_radius);

            if ((dist - harvester_component->resource_entity->entity->unit_radius) <= harvester_component->harvesting_distance)
            {
                game_entity->entity->should_move_to_pos = 0;
                harvester_component->state = HARVESTING_STATE_HARVESTING;
            }
        }

        break;

    case HARVESTING_STATE_HARVESTING:
        // printf("Harvesting\n");
        if (harvester_component->resource_quantity == harvester_component->max_resource_quantity)
        {
            harvester_component->state = HARVESTING_STATE_DEPOSITING;
            break;
        }

        Component_Resource *resource_component = harvester_component->resource_entity->resource_component;
        int harvest_result = harvest_resource(harvester_component->resource_entity, game_entity);

        if (harvest_result == 2)
        {
            Game_Entity *nearest_resource = find_nearest_free_resource(game_entity);
            if (!nearest_resource)
            {
                harvester_component->state = HARVESTING_STATE_IDLE;
                break;
            }
            harvester_component->resource_entity = nearest_resource;
            harvester_component->state = HARVESTING_STATE_COMMUTING;
            break;
        }
        else if (harvest_result == 0)
        {
            if (harvester_component->harvesting_time_left == 0)
            {
                harvester_component->harvesting_time_left = harvester_component->harvesting_speed;
            }
            else
            {
                harvester_component->harvesting_time_left--;

                if (harvester_component->harvesting_time_left == 0)
                {
                    harvester_component->resource_quantity = 5;
                    harvester_component->state = HARVESTING_STATE_DEPOSITING;
                }
            }
        }

        break;

    case HARVESTING_STATE_DEPOSITING:
        // printf("Depositing\n");
        if (!game_entity->entity->should_move_to_pos)
        {
            Array *entities = global.render.entities;

            Game_Entity *closest_building = NULL;
            float closest_distance = INFINITY;

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
                    float dist = sqrtf(powf(game_entity->entity->pos[0] - entity_class->entity->pos[0], 2) + powf(game_entity->entity->pos[1] - entity_class->entity->pos[1], 2));

                    if (dist < closest_distance)
                    {
                        closest_distance = dist;
                        closest_building = entity_class;
                    }
                }
            }

            harvester_component->return_building_entity = closest_building;
            move_to(game_entity->entity, closest_building->entity->pos);
            break;
        }
        else
        {
            float dist = sqrtf(powf(game_entity->entity->pos[0] - game_entity->entity->move_to_pos[0], 2) + powf(game_entity->entity->pos[1] - game_entity->entity->move_to_pos[1], 2));

            if ((dist - harvester_component->return_building_entity->entity->unit_radius) <= harvester_component->harvesting_distance)
            {
                game_global.game_stores.in_game_store.player->crystal_count += harvester_component->resource_quantity;

                game_entity->entity->should_move_to_pos = 0;
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