#include "units.h"

#include "../../../engine/engine.h"
#include "../../../generic/generic.h"

#include "../game_entities_internal.h"
#include "../components/components.h"

Game_Entity *create_resource(RESOURCE type, vec3 pos)
{
    /** Initializing data */
    int offset[2] = {1, 1};
    int sprite_sheet_size[2] = {1, 1};
    int sprite_size[2] = {64, 64};
    vec2 size = {1, 1};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_RESOURCE;
    add_collision_data(entity, 0.3);

    /** Resource */
    Game_Entity *resource = create_game_entity(entity);
    add_resource_component(resource, type, 1000);
    add_selectable_component(resource, _UNIT_TYPE);

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_VERTICAL_QUAD, SHADER_DEFAULT, "assets/props/Props_Sheet.png");
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        render_item->is_static_object = 1;

        add_sprite_sheet_data(render_item, offset, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, size, DEFAULT_UNIT_ROTATION, NULL);
        bind_render_item_data(render_item);
    }

    entity->render_item = render_item;
    add_entity(entity);
    return resource;
};
