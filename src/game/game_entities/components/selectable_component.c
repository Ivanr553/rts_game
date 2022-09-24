#include "components.h"

#include "../game_entities_internal.h"

void add_selectable_component(Game_Entity *game_entity, GAME_ENTITY_TYPE unit_type)
{
    Component_Selectable *selectable_component = calloc(1, sizeof(Component_Selectable));
    selectable_component->is_selected = 0;

    game_entity->selectable_component = selectable_component;
    game_entity->unit_type = unit_type;
}