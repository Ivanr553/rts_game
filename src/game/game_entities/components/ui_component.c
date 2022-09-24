#include "components.h"

#include "../game_entities_internal.h"

void add_ui_component(Game_Entity *game_entity, UI_TYPE type, Game_Entity *parent_entity, void (*on_click)(Game_Entity *), void (*update_ui_entity)(Game_Entity *))
{
    Component_UI *ui_component = calloc(1, sizeof(Component_UI));
    ui_component->on_click = on_click;
    ui_component->type = type;
    ui_component->update_ui_entity = update_ui_entity;
    ui_component->game_entity_pointer = parent_entity;

    game_entity->ui_component = ui_component;
    game_entity->entity->is_ui_entity = 1;
};