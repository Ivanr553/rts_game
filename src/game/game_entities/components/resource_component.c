#include "components.h"

#include "../game_entities_internal.h"
#include "../../state/state.h"

void update_resource_component(Game_Entity *game_entity)
{
    Component_Resource *resource_component = game_entity->resource_component;
    if (resource_component->recently_harvested_count > 0)
    {
        resource_component->recently_harvested_count--;

        if (resource_component->recently_harvested_count == 0)
        {
            if (resource_component->standby_harvesting_id != 0)
            {
                resource_component->harvesting_id = resource_component->standby_harvesting_id;
                resource_component->standby_harvesting_id = 0;
                resource_component->recently_harvested_count = 10;
            }
        }
    }
}

void add_resource_component(Game_Entity *game_entity, RESOURCE resource_type, int amount)
{
    Component_Resource *resource_component = calloc(1, sizeof(Component_Resource));
    resource_component->resource_type = resource_type;
    resource_component->amount = amount;
    resource_component->update_resource_component = update_resource_component;

    game_entity->resource_component = resource_component;
};

/** 0 = successful harvest | 1 = standby harvest | 2 = cannot harvest */
int harvest_resource(Game_Entity *resource, Game_Entity *harvester)
{
    Component_Resource *resource_component = resource->resource_component;
    long id = harvester->entity->id;

    if (resource_component->harvesting_id == id)
    {
        resource_component->recently_harvested_count = 10;

        return 0;
    }
    else if (resource_component->standby_harvesting_id == id)
    {
        /** Idle until the first slot is freed up */
        return 1;
    }

    if (resource_component->harvesting_id == 0)
    {
        resource_component->harvesting_id = id;
        return 0;
    }
    else if (resource_component->standby_harvesting_id == 0)
    {
        resource_component->standby_harvesting_id = id;
        return 1;
    }

    return 2;
}

short is_harvestable(Game_Entity *resource, Game_Entity *harvester)
{
    Component_Resource *resource_component = resource->resource_component;
    long id = harvester->entity->id;

    if (resource_component->harvesting_id == 0 || resource_component->harvesting_id == id)
    {
        return 1;
    }
    else if (resource_component->standby_harvesting_id == 0 || resource_component->standby_harvesting_id == id)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

Game_Entity *find_nearest_free_resource(Game_Entity *game_entity)
{
    Array *entities = global.render.entities;

    Game_Entity *closest_resource = NULL;
    float closest_distance = INFINITY;

    for (int i = 0; i < entities->len; i++)
    {
        Entity **entity_ptr_ptr = get_item_from_array(entities, i);
        Entity *entity = *entity_ptr_ptr;
        Game_Entity *looped_game_entity = entity->entity_class;

        if (!looped_game_entity)
        {
            continue;
        }

        if (looped_game_entity && looped_game_entity->resource_component && is_harvestable(looped_game_entity, game_entity))
        {
            float dist = sqrtf(powf(game_entity->entity->pos[0] - looped_game_entity->entity->pos[0], 2) + powf(game_entity->entity->pos[1] - looped_game_entity->entity->pos[1], 2));

            if (dist < closest_distance)
            {
                closest_distance = dist;
                closest_resource = looped_game_entity;
            }
        }
    }

    return closest_resource;
}