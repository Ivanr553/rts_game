#include "worker.h"

#include "../../engine/engine.h"
#include "../../generic/generic.h"

#include "game_entities_internal.h"
#include "components/components.h"

Game_Entity *create_worker(vec3 pos, char *file_path)
{
    /** Initializing data */
    int offset[2] = {1, 1};
    int sprite_sheet_size[2] = {6, 2};
    int sprite_size[2] = {64, 64};
    vec2 size = {1, 1};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UNIT;
    entity->update_entity = update_worker;

    /** Base Class*/
    Game_Entity *worker = create_game_entity(entity);
    worker->is_selectable = 1;
    worker->is_selected = 0;
    add_harvester_component(worker, 100, 0.3);

    add_movement_data(entity, (vec3){DEFAULT_UNIT_MAX_VELOCITY, DEFAULT_UNIT_MAX_VELOCITY, DEFAULT_UNIT_MAX_VELOCITY});
    add_collision_data(entity, 0.3);

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_VERTICAL_QUAD, SHADER_DEFAULT, file_path);
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        /** Entity */
        add_worker_animations(entity);

        /** Render Item */
        add_sprite_sheet_data(render_item, offset, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, size, NULL, NULL);
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

void handle_selection(Game_Entity *worker)
{
    Entity *select_entity = get_bound_entity(worker->entity, BOUND_ENTITY_SELECTOR);

    if (worker->is_selected == 1)
    {
        if (!select_entity)
        {
            select_entity = create_entity(worker->entity, worker->entity->pos);
            Render_Item *render_item = get_render_item(1, RENDER_ITEM_CIRCLE, SHADER_COLOR, EMPTY_TEXTURE);
            append_item_to_render_item(render_item, select_entity);
            init_render_item(render_item, (vec3){worker->entity->pos[0], worker->entity->pos[1], 0.05}, (vec2){0.5, 0.5}, NULL, (vec4){0, 0, 0, 1});
            bind_render_item_data(render_item);

            select_entity->render_item = render_item;
            select_entity->entity_class_type = 100;
            add_entity(select_entity);
            add_bound_entity(worker->entity, BOUND_ENTITY_SELECTOR, select_entity);
        }

        if (worker->entity->render_item->should_update)
        {
            select_entity->pos[0] = worker->entity->pos[0];
            select_entity->pos[1] = worker->entity->pos[1];
            select_entity->pos[2] = 0.05;
            select_entity->render_item->should_update = 1;
            select_entity->render_item->updated = 1;
        }
    }
}

void update_worker(Entity *entity)
{
    Game_Entity *worker = entity->entity_class;

    // handle_selection(worker);
};