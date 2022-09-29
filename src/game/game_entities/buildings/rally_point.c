#include "building.h"

#include "../game_entities_internal.h"
#include "../../../engine/engine.h"
#include "../components/components.h"
#include "../../game_map/game_map.h"

void update_rally_point(Entity *rally_point)
{
    Entity *bound_entity = rally_point->bound_entity;

    if (!bound_entity)
    {
        printf("Bound entity pointing to null entity\n");
        remove_entity(rally_point);
        return;
    }

    Game_Entity *bound_entity_entity_class = bound_entity->entity_class;
    if (!bound_entity_entity_class || !bound_entity_entity_class->selectable_component->is_selected)
    {
        remove_entity(rally_point);
        return;
    }

    rally_point->pos[0] = bound_entity_entity_class->building_component->rally_point[0];
    rally_point->pos[1] = bound_entity_entity_class->building_component->rally_point[1];
    rally_point->pos[2] = 0.05;
    rally_point->render_item->should_update = 1;
    rally_point->render_item->updated = 1;
}

void create_rally_point(Entity *entity_to_bind_to)
{
    if(!entity_to_bind_to)
    {
        printf("Null entity provided to create rally point\n");
        return;
    }

    Game_Entity *game_entity = entity_to_bind_to->entity_class;

    if(!game_entity->building_component || !game_entity->building_component->can_rally)
    {
        printf("Game entity does not have a building component or cannot support a rally point in create rally point\n");
        return;
    }

    Component_Building *building_component = game_entity->building_component;

    Entity *rally_point = create_entity(NULL, building_component->rally_point);
    rally_point->size[0] = 0.1;
    rally_point->size[1] = 0.1;
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_COLOR, EMPTY_TEXTURE);
    append_item_to_render_item(render_item, rally_point);
    init_render_item(render_item, (vec3){rally_point->pos[0], rally_point->pos[1], 0.05}, rally_point->size, NULL, (vec4){0, 1, 0, 1}, NULL, NULL, 1);
    bind_render_item_data(render_item);

    rally_point->render_item = render_item;
    rally_point->bound_entity = entity_to_bind_to;
    rally_point->update_entity = update_rally_point;
    add_entity(rally_point);
}
