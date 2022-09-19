#include "../../../engine/engine.h"
#include "../../../generic/generic.h"
#include "../../game_entities/game_entities.h"

#include "events.h"

short handle_ui_element_click(float x, float y)
{
    Array *entities = global.render.ui_entities;

    for (int i = 0; i < entities->len; i++)
    {
        Entity **entity_ptr_ptr = get_item_from_array(entities, i);
        Entity *entity = *entity_ptr_ptr;
        Game_Entity *game_entity = entity->entity_class;

        if (is_point_within_square(entity->pos[0], entity->pos[1], entity->render_item->size[0], -entity->render_item->size[1], x, y))
        {

            if (game_entity->ui_component->on_click)
            {
                game_entity->ui_component->on_click(game_entity);
            }

            return 1;
        }
    }

    return 0;
}