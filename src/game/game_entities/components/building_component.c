#include "components.h"

#include "../../../engine/engine.h"
#include "../game_entities_internal.h"

void add_building_component(Game_Entity *unit_entity, BUILDING_TYPE building_type)
{
    Component_Building *building_component = calloc(1, sizeof(Component_Building));
    building_component->building_type = building_type;

    unit_entity->building_component = building_component;
}