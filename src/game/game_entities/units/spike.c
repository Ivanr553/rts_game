#include "units.h"

#include "../../../engine/engine.h"
#include "../../../generic/generic.h"

#include "../game_entities_internal.h"
#include "../components/components.h"
#include "../../state/state.h"

Unit_Data get_unit_data_by_type(GAME_ENTITY_TYPE type)
{
    switch (type)
    {
    case GAME_ENTITY_TYPE_SPIKE:
    {
        Unit_Data data = {
            .crystal_cost = 100,
            .build_time = 800,
            .health = 100,
            .size = {1.1, 1.1},
            .unit_radius = 0.4,
            .energy = 0,
            .armor = 1,
            .weapon_1 = WEAPON_TYPE_BULLET,
            .weapon_2 = _WEAPON_TYPE,
            .file_path = "assets/units/SPIKE.png"};

        return data;
    }
    default:
        return;
    }
}

Game_Entity *create_unit(vec3 pos, GAME_ENTITY_TYPE unit_type)
{
    /** Initializing data */
    int sprite_sheet_size[2] = {6, 2};
    int sprite_size[2] = {64, 64};

    Unit_Data data = get_unit_data_by_type(unit_type);

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UNIT;
    // entity->update_entity = update_worker;
    entity->offset[0] = 1;
    entity->offset[1] = 1;
    entity->size[0] = data.size[0];
    entity->size[1] = data.size[1];

    /** Base Class*/
    Game_Entity *unit = create_game_entity(entity, game_global.game_stores.in_game_store.player->player_slot);
    add_selectable_component(unit, unit_type);
    add_combat_component(unit, data.health, data.energy, data.armor, data.weapon_1, data.weapon_2);

    add_movement_data(entity, (vec3){DEFAULT_UNIT_MAX_VELOCITY, DEFAULT_UNIT_MAX_VELOCITY, DEFAULT_UNIT_MAX_VELOCITY});
    add_collision_data(entity, data.unit_radius);

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_VERTICAL_QUAD, SHADER_DEFAULT, data.file_path);
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        /** Entity */
        // add_worker_animations(entity);

        /** Render Item */
        add_sprite_sheet_data(render_item, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, entity->size, NULL, NULL, entity->offset, NULL, 1);
        bind_render_item_data(render_item);
    }

    // entity->animation_data = calloc(1, sizeof(Animation_Data));
    // set_animation(entity, DEFAULT_ANIMATION_WALKING);
    unit->entity = entity;

    add_entity(entity);
    return unit;
}