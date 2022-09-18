#include "components.h"

#include "../game_entities_internal.h"

void add_resource_component(Game_Entity *unit_entity, RESOURCE resource_type, int amount)
{
    Component_Resource *resource_component = calloc(1, sizeof(Component_Resource));
    resource_component->resource_type = resource_type;
    resource_component->amount = amount;

    unit_entity->resource_component = resource_component;
};