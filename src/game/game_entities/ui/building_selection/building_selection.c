#include "building_selection.h"

#include "../../../../engine/engine.h"
#include "../../../state/game_global.h"
#include "../../game_entities_internal.h"
#include "../../buildings/building.h"

void show_building_selection(int building_width, int building_height)
{
    float width = building_width;
    float height = building_height;
    vec2 size = {width, height};
    vec3 pos = {1 - width / 2, -height * 0.95, 0.01};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->update_entity = update_build_selection;

    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_COLOR, EMPTY_TEXTURE);
    append_item_to_render_item(render_item, entity);

    /** Render Item */
    init_render_item(render_item, pos, size, NULL, (vec4){0, 0, 1, 0.2});
    bind_render_item_data(render_item);

    Game_Entity *building = create_building(pos, size, BUILDING_TYPE_BASE);
    add_bound_entity(entity, BOUND_ENTITY_BUILDING_PLACEMENT, building->entity);

    add_entity(entity);
    game_global.game_stores.in_game_store.building_selection_entity_id = entity->id;
};

void hide_building_selection(void)
{
    Entity *entity = get_entity_by_id(game_global.game_stores.in_game_store.building_selection_entity_id);
    remove_entity(entity);
    game_global.game_stores.in_game_store.building_selection_entity_id = NULL;
};

void update_build_selection(Entity *entity)
{
    float mouse_pos[3];
    get_mouse_pos_on_map(mouse_pos);

    snap_to_map_grid(entity->pos, entity->render_item->size);

    entity->render_item->should_update = 1;
}