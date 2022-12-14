#include "units.h"

#include "../../../engine/engine.h"
#include "../../../generic/generic.h"

#include "../game_entities_internal.h"
#include "../components/components.h"
#include "../../state/state.h"

Game_Entity *create_resource(RESOURCE type, vec3 pos)
{
    /** Initializing data */
    int sprite_sheet_size[2] = {1, 1};
    int sprite_size[2] = {64, 64};
    vec2 size = {1, 1};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_RESOURCE;
    entity->is_fixed_object = 1;
    entity->offset[0] = 1;
    entity->offset[1] = 1;
    entity->size[0] = size[0];
    entity->size[1] = size[1];
    add_collision_data(entity, 0.3);

    /** Resource */
    Game_Entity *resource = create_game_entity(entity, game_global.game_stores.in_game_store.player->player_slot);
    add_resource_component(resource, type, 1000);
    add_selectable_component(resource, _GAME_ENTITY_TYPE);

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_VERTICAL_QUAD, SHADER_DEFAULT, "assets/props/Props_Sheet.png");
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        add_sprite_sheet_data(render_item, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, entity->size, DEFAULT_UNIT_ROTATION, NULL, entity->offset, NULL, 1);
        bind_render_item_data(render_item);
    }

    entity->render_item = render_item;
    add_entity(entity);
    return resource;
};
