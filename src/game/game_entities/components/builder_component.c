#include "components.h"

#include "../../../engine/engine.h"
#include "../game_entities_internal.h"

void update_builder_component(Game_Entity *game_entity)
{

}

void add_builder_component(Game_Entity *game_entity, int build_speed)
{
    Component_Builder *builder_component = calloc(1, sizeof(Component_Builder));
    builder_component->build_speed = build_speed;
    builder_component->update_builder_component = update_builder_component;

    game_entity->builder_component = builder_component;
}