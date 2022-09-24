#include "components.h"

#include "../game_entities_internal.h"
#include "../../state/state.h"

void add_resource_component(Game_Entity *game_entity, RESOURCE resource_type, int amount)
{
    Component_Resource *resource_component = calloc(1, sizeof(Component_Resource));
    resource_component->resource_type = resource_type;
    resource_component->amount = amount;

    game_entity->resource_component = resource_component;
};

void resource_start_harvesting(Game_Entity *game_entity, long entity_id)
{
    Component_Resource *resource_component = game_entity->resource_component;
    resource_component->recently_harvested_by_id = entity_id;
    resource_component->recently_harvested_count = 10;
}

Game_Entity *find_nearest_free_resource(Game_Entity *game_entity)
{
    Array *entities = global.render.entities;

    for (int i = 0; i < entities->len; i++)
    {
        Entity **entity_ptr_ptr = get_item_from_array(entities, i);
        Entity *entity = *entity_ptr_ptr;
        Game_Entity *game_entity = entity->entity_class;

        if(!game_entity)
        {
            continue;
        }

        if (game_entity->resource_component)
        {
            if (game_entity->resource_component->recently_harvested_count == 0)
            {
                return game_entity;
            }
        }
    }

    return NULL;
}