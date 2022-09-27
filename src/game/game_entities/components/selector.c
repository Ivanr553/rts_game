#include "components.h"

#include "../../../engine/engine.h"
#include "../game_entities_internal.h"

void update_selection(Entity *select_entity)
{
    Entity *bound_entity = select_entity->bound_entity;

    if (!bound_entity)
    {
        printf("Bound entity pointing to null entity\n");
        return;
    }

    Game_Entity *bound_entity_entity_class = bound_entity->entity_class;
    if (!bound_entity_entity_class || !bound_entity_entity_class->selectable_component->is_selected)
    {
        remove_entity(select_entity);
        return;
    }

    select_entity->pos[0] = bound_entity->pos[0];
    select_entity->pos[1] = bound_entity->pos[1];
    select_entity->pos[2] = 0.05;
    select_entity->render_item->should_update = 1;
    select_entity->render_item->updated = 1;
}

void create_selection(Entity *entity_to_bind_to)
{
    Entity *select_entity = create_entity(NULL, entity_to_bind_to->pos);
    select_entity->size[0] = entity_to_bind_to->unit_radius * 2;
    select_entity->size[1] = entity_to_bind_to->unit_radius * 2;
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_CIRCLE, SHADER_COLOR, EMPTY_TEXTURE);
    append_item_to_render_item(render_item, select_entity);
    init_render_item(render_item, (vec3){entity_to_bind_to->pos[0], entity_to_bind_to->pos[1], 0.05}, select_entity->size, NULL, (vec4){0, 0, 0, 1}, NULL, NULL, 1);
    bind_render_item_data(render_item);

    select_entity->render_item = render_item;
    select_entity->entity_class_type = 100;
    select_entity->bound_entity = entity_to_bind_to;
    select_entity->update_entity = update_selection;
    add_entity(select_entity);
}