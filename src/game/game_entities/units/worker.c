#include "units.h"

#include "../../../engine/engine.h"
#include "../../../generic/generic.h"

#include "../game_entities_internal.h"
#include "../components/components.h"
#include "../../state/state.h"

char *get_worker_data(GAME_ENTITY_TYPE unit_type)
{
    switch (unit_type)
    {
    case GAME_ENTITY_TYPE_MAX:
        return "assets/units/MAX.png";

    case GAME_ENTITY_TYPE_ALF:
        return "assets/units/Alf.png";

    default:
        return NULL;
    }
}

Game_Entity *create_worker(vec3 pos, GAME_ENTITY_TYPE unit_type)
{
    /** Initializing data */
    int sprite_sheet_size[2] = {6, 2};
    int sprite_size[2] = {64, 64};
    vec2 size = {1, 1};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UNIT;
    // entity->update_entity = update_worker;
    entity->offset[0] = 1;
    entity->offset[1] = 1;
    entity->size[0] = size[0];
    entity->size[1] = size[1];

    /** Base Class*/
    Game_Entity *worker = create_game_entity(entity, game_global.game_stores.in_game_store.player->player_slot);
    add_selectable_component(worker, unit_type);
    add_harvester_component(worker, 200, 0.3);
    add_combat_component(worker, 50, 0, 0, WEAPON_TYPE_MELEE, _WEAPON_TYPE);
    add_builder_component(worker, 1);

    add_movement_data(entity, (vec3){DEFAULT_UNIT_MAX_VELOCITY, DEFAULT_UNIT_MAX_VELOCITY, DEFAULT_UNIT_MAX_VELOCITY});
    add_collision_data(entity, 0.3);

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_VERTICAL_QUAD, SHADER_DEFAULT, get_worker_data(unit_type));
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        /** Entity */
        add_worker_animations(entity);

        /** Render Item */
        add_sprite_sheet_data(render_item, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, entity->size, NULL, NULL, entity->offset, NULL, 1);
        bind_render_item_data(render_item);
    }

    entity->animation_data = calloc(1, sizeof(Animation_Data));
    set_animation(entity, DEFAULT_ANIMATION_WALKING);
    worker->entity = entity;

    add_entity(entity);
    return worker;
};

void add_worker_animations(Entity *entity)
{
    Animation *idle_animation = calloc(1, sizeof(Animation));
    idle_animation->animation_name = DEFAULT_ANIMATION_IDLE;
    idle_animation->total_frames = 5;
    idle_animation->frame_rate = DEFAULT_ANIMATION_SPEED;
    idle_animation->should_loop = 1;
    idle_animation->animation_pos = 0;

    add_animation(entity->render_item, idle_animation);

    Animation *walking_animation = calloc(1, sizeof(Animation));
    walking_animation->animation_name = DEFAULT_ANIMATION_WALKING;
    walking_animation->total_frames = 6;
    walking_animation->frame_rate = DEFAULT_ANIMATION_SPEED;
    walking_animation->should_loop = 1;
    walking_animation->animation_pos = 1;

    add_animation(entity->render_item, walking_animation);
};

void update_worker(Entity *entity)
{
    Game_Entity *worker = entity->entity_class;
};