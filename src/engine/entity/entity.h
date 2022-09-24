#pragma once

#include <linmath.h>

long entity_id_counter;
int removed_entity_count;

#define DEFAULT_BOUND_ENTITY_MAP_LENGTH 10

typedef struct entity
{
    /** Pointer to parent class*/
    void *entity_class;
    int entity_class_type;

    /** Function pointer */
    void (*update_entity)(void *);

    /** METADATA */
    short should_ignore_camera;
    int entity_array_pos;
    int non_static_entity_array_pos;
    int ui_entity_array_pos;
    int render_item_array_pos;
    short is_ui_entity;

    /** Movement */
    short should_move_to_pos;
    float move_to_pos[3];
    int stop_should_move_count;

    /** Physics */
    long id;
    float pos[3];
    float velocity[3];
    float acceleration[3];
    float max_velocity[3];
    float default_acceleration[3];
    float unit_radius;
    short can_collide;
    short has_collided;
    short is_fixed_object;

    /** Animation */
    Animation_Data *animation_data;

    /** RENDERING */
    int vbo_pos;
    vec2 size;
    Render_Item *render_item;
    int offset[2];
    float points[12];

    /** Bound entities */
    Map *bound_entity_map;
    void *bound_entity;

} Entity;

typedef struct entity_map
{
    Map *_entity_map;
    Array *free_ids;
} Entity_Map;

/** Core */
Entity *create_entity(void *entity_class, vec3 pos);
void *add_entity(Entity *entity);
void remove_entity(Entity *entity);
void add_entity_to_map(Entity *entity);
void remove_entity_from_map(Entity *entity);
Entity *get_entity_by_id(long id);
void *add_bound_entity(Entity *entity, char *bound_entity_name, Entity *bound_entity);

/** Movement */
Entity *move_to(Entity *entity, vec3 pos);

/** Adding Data */
Entity *set_animation(Entity *entity, char *animation_name);
void add_movement_data(Entity *entity, vec3 max_velocity);
void add_collision_data(Entity *entity, float unit_radius);

/** MISC */
Entity *create_line_entity(vec3 pos, float vertices[6], vec4 color);
void cleanup_entities(void);
