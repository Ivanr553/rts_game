#include <stdlib.h>

#include "../../engine/engine.h"

#include "entity.h"
#include "../../generic/generic.h"
#include "../global.h"

long entity_id_counter = 1;

/** Use to keep track of when we should clean up the entity array */
int removed_entity_count = 0;

Entity *create_entity(void *entity_class, vec3 pos)
{
    Entity *entity = calloc(1, sizeof(Entity));
    entity->id = entity_id_counter++;
    entity->entity_class = entity_class;
    entity->bound_entity_map = create_map(DEFAULT_BOUND_ENTITY_MAP_LENGTH, 0);

    entity->pos[0] = pos[0];
    entity->pos[1] = pos[1];
    entity->pos[2] = pos[2];

    return entity;
}

void *add_entity(Entity *entity)
{
    if (!entity)
    {
        printf("Attempted to add NULL entity\n");
        return NULL;
    }

    if (!entity->render_item->is_static_object)
    {
        entity->non_static_entity_array_pos = global.render.non_static_entities->len;
        // printf("non_static_entity_array_pos pos: %d\n", entity->non_static_entity_array_pos);
        append_array(global.render.non_static_entities, &entity);
    }

    if (entity->is_ui_entity)
    {
        entity->ui_entity_array_pos = global.render.ui_entities->len;
        printf("Added entity to ui entities\n");
        append_array(global.render.ui_entities, &entity);
    }

    entity->entity_array_pos = global.render.entities->len;
    // printf("entity pos: %d\n", entity->entity_array_pos);
    if (!append_array(global.render.entities, &entity))
    {
        return NULL;
    }

    add_entity_to_map(entity);

    // printf("Finished adding entity\n");
    return entity;
};

void remove_entity(Entity *entity)
{
    if (!entity)
    {
        printf("Attempted to remove NULL entity\n");
        return;
    }
    removed_entity_count++;

    int vbo_pos = entity->vbo_pos;
    int entity_array_pos = entity->entity_array_pos;
    int non_static_entity_array_pos = entity->non_static_entity_array_pos;
    int ui_entity_array_pos = entity->ui_entity_array_pos;
    int entity_ids_len = entity->render_item->entity_ids->len;

    if (entity_ids_len == 1)
    {
        int shader_pos = entity->render_item->shader_position;
        int texture_location = entity->render_item->texture_data->texture_location;
        int index = entity->render_item->render_item_index;

        remove_items_from_array_and_shift(global.render.shader_groups[shader_pos].texture_groups[texture_location].render_items, index, 1);

        for (int i = 0; i < global.render.shader_groups[shader_pos].texture_groups[texture_location].render_items->len; i++)
        {
            Render_Item *render_item = get_item_from_array(global.render.shader_groups[shader_pos].texture_groups[texture_location].render_items, i);
            render_item->render_item_index = i;
            for (int e = 0; e < render_item->entity_ids->len; e++)
            {
                Entity *entity = get_entity_by_id(*((long *)get_item_from_array(render_item->entity_ids, e)));
                entity->render_item = render_item;
            }
        }
        printf("Removed: %d\n", index);
    }
    else
    {
        Render_Item *render_item = entity->render_item;
        remove_items_from_array_and_shift(render_item->entity_ids, vbo_pos, 1);

        switch (render_item->type)
        {
        case RENDER_ITEM_QUAD:
        case RENDER_ITEM_VERTICAL_QUAD:
            render_item->vertices_len = render_item->entity_ids->len * DEFAULT_QUAD_VERTICES_LEN;
            render_item->indices_len = render_item->entity_ids->len * DEFAULT_QUAD_INDICES_LEN;
            render_item->positional_vertices_len = DEFAULT_QUAD_STRIDE * render_item->entity_ids->len;

            render_item->vertices = realloc(render_item->vertices, DEFAULT_QUAD_VERTICES_LEN * render_item->entity_ids->len * sizeof(f32));
            render_item->positional_vertices = realloc(render_item->positional_vertices, DEFAULT_QUAD_STRIDE * render_item->entity_ids->len * sizeof(f32));
            render_item->indices = realloc(render_item->indices, DEFAULT_QUAD_INDICES_LEN * render_item->entity_ids->len * sizeof(u32));

            for (int i = 0; i < render_item->entity_ids->len; i++)
            {
                long id = *((long *)get_item_from_array(render_item->entity_ids, i));
                Entity *_entity = get_entity_by_id(id);
                _entity->vbo_pos = i;

                update_render_item(_entity);
            }
            break;

        default:
            printf("Unable to remap entities when removing entity: %d\n", entity->id);
            break;
        }
    }

    if (!entity->render_item->is_static_object)
    {
        remove_items_from_array_and_shift(global.render.non_static_entities, non_static_entity_array_pos, 1);
        for (int i = 0; i < global.render.non_static_entities->len; i++)
        {
            Entity *_entity = *((Entity **)get_item_from_array(global.render.non_static_entities, i));
            _entity->non_static_entity_array_pos = i;
        }
    }

    if (entity->is_ui_entity)
    {
        remove_items_from_array_and_shift(global.render.ui_entities, ui_entity_array_pos, 1);
        for (int i = 0; i < global.render.ui_entities->len; i++)
        {
            Entity *_entity = *((Entity **)get_item_from_array(global.render.ui_entities, i));
            _entity->ui_entity_array_pos = i;
        }
    }

    remove_items_from_array_and_shift(global.render.entities, entity_array_pos, 1);
    for (int i = 0; i < global.render.entities->len; i++)
    {
        Entity *_entity = *((Entity **)get_item_from_array(global.render.entities, i));
        _entity->entity_array_pos = i;
    }
    remove_entity_from_map(entity);

    memset(entity, 0, sizeof(Entity));
    free(entity);
}

void add_entity_to_map(Entity *entity)
{
    if (!entity)
    {
        printf("Attempted to add NULL entity to map\n");
        return;
    }

    Entity *entity_from_map_ptr = set_value_in_map(global.render.entity_map._entity_map, entity->id, entity);
};

void remove_entity_from_map(Entity *entity)
{
    if (!entity)
    {
        printf("Attempted to remove NULL entity from map\n");
        return;
    }

    remove_value_in_map(global.render.entity_map._entity_map, entity->id, entity);
};

void *add_bound_entity(Entity *entity, char *bound_entity_name, Entity *bound_entity)
{
    if (strlen(bound_entity_name) == 0)
    {
        printf("Empty bound entity name when trying to get bound_entity\n");
        return NULL;
    }

    printf("Binding entity\n");
    return set_value_in_map(entity->bound_entity_map, bound_entity_name, bound_entity);
}

Entity *get_entity_by_id(long id)
{
    if (id == 0)
    {
        return NULL;
    }

    Entity *entity = get_value_from_map(global.render.entity_map._entity_map, id);
    return entity;
}

Entity *move_to(Entity *entity, vec3 pos)
{
    if (!entity)
    {
        printf("Attempted to move NULL entity\n");
        return NULL;
    }

    entity->should_move_to_pos = 1;

    entity->move_to_pos[0] = pos[0];
    entity->move_to_pos[1] = pos[1];
    entity->move_to_pos[2] = pos[2];

    /** Update unit rotation when moving in different directions */
    // entity->render_item->rotation[1] = M_PI;

    // printf("Moving unit to: %f %f %f\n", pos[0], pos[1], pos[2]);
    return entity;
}

/** ENTITY DATA */
void add_movement_data(Entity *entity, vec3 max_velocity)
{
    // entity->stop_should_move_count = 0;

    entity->max_velocity[0] = DEFAULT_UNIT_MAX_VELOCITY;
    entity->max_velocity[1] = DEFAULT_UNIT_MAX_VELOCITY;
    entity->max_velocity[2] = DEFAULT_UNIT_MAX_VELOCITY;

    entity->acceleration[0] = 0;
    entity->acceleration[1] = 0;
    entity->acceleration[2] = 0;
}

void add_collision_data(Entity *entity, float unit_radius)
{
    entity->can_collide = 1;
    entity->unit_radius = unit_radius;
}

Entity *set_animation(Entity *entity, char *animation_name)
{
    if (!entity)
    {
        printf("Attempted to set animation for NULL entity\n");
        return NULL;
    }

    // printf("Setting animation\n");
    if (!entity)
    {
        printf("No render item passed to set_animation\n");
        return NULL;
    }

    if (!entity->render_item->animations)
    {
        printf("Unable to set animation, no animation_data has been initialized\n");
        return NULL;
    }

    Animation *animation = get_value_from_map(entity->render_item->animations, animation_name);

    if (!animation)
    {
        printf("Unable to get animation with name %s when trying to set animation\n", animation_name);
        return NULL;
    }

    entity->animation_data->current_animation = animation;
    entity->animation_data->current_frame = 0;
    entity->offset[1] = animation->animation_pos + 1;
    // printf("Finished Setting animation\n");

    return entity;
};

void cleanup_entities(void){};

/** GENERIC ENTITY CREATION */

Entity *create_line_entity(vec3 pos, float vertices[6], vec4 color)
{
    Entity *line = create_entity(NULL, pos);
    line->can_collide = 1;
    Render_Item *render_item = create_render_item(RENDER_ITEM_LINE);
    add_shader(render_item, 2);
    line->id = entity_id_counter++;

    vec2 size = {1, 1};

    init_render_item(render_item, pos, size, NULL, color, NULL, NULL, 1);
    create_line(render_item, vertices);
    bind_render_item_data(render_item);

    line->render_item = render_item;

    return line;
};