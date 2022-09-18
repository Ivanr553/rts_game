#include "components.h"

#include "../game_entities_internal.h"

void add_ui_component(Game_Entity *game_entity, void (*on_click)())
{
    Component_UI *ui_component = calloc(1, sizeof(Component_UI));
    ui_component->on_click = on_click;

    game_entity->ui_component = ui_component;
    game_entity->entity->is_ui_entity = 1;
};