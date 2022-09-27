#pragma once

#include <linmath.h>
#include <glad/glad.h>

#include "../../util/util.h"
#include "../../types.h"
#include "../../../generic/generic.h"

#define DEFAULT_ANIMATION_SPEED 23
#define DEFAULT_UNIT_MAX_VELOCITY 0.025
#define DEFAULT_UNIT_ACCELERATION 0.0005
#define DEFAULT_UNIT_RADIUS 0.3
#define DEFAULT_UNIT_Z 0.5
#define DEFAULT_BUILDING_Z 0.65

#define TILE_SPRITE_SHEET_COUNT 30
#define TILE_SPRITE_SIZE 16

#define DEFAULT_ANIMATION_MAP_SIZE 10

#define DEFAULT_QUAD_VERTICES_LEN 48
#define DEFAULT_QUAD_INDICES_LEN 6
#define DEFAULT_QUAD_STRIDE 12

#define DEFAULT_CIRCLE_VERTICES_AMOUNT 60
#define DEFAULT_POS_ONLY_VERTEX_STRIDE 3

typedef enum render_item_type
{
    _RENDER_ITEM,
    RENDER_ITEM_QUAD,
    RENDER_ITEM_VERTICAL_QUAD,
    RENDER_ITEM_RECT,
    RENDER_ITEM_CUBE,
    RENDER_ITEM_LINE,
    RENDER_ITEM_CIRCLE,
    RENDER_ITEM_QUAD_WITH_POINTS
} RENDER_ITEM_TYPE;

typedef struct vertex
{
    f32 pos[3];
    f32 uvs[2];
    f32 color[4];
    f32 normal[3];
} Vertex;

typedef struct texture_data
{
    GLuint *texture;
    short texture_location;
    char *file_path;
    int width;
    int height;
    int color_channels;
} Texture_Data;

typedef struct sprite_sheet_data
{
    int sprite_size[2];
    int sprite_sheet_size[2];
} Sprite_Sheet_Data;

typedef struct animation
{
    char *animation_name;
    int total_frames;
    int frame_rate;
    short should_loop;
    int animation_pos;
} Animation;

typedef struct animation_data
{
    Animation *current_animation;
    int current_frame;
} Animation_Data;

typedef struct render_item
{
    /** Metadata */
    long id;
    long render_item_index;
    RENDER_ITEM_TYPE type;
    short should_update;
    short updated;
    short should_ignore_camera;
    short cleanup;
    short hide;

    /** Core Rendering objects */
    u32 *vbo;
    u32 *uv_vbo;
    u32 *vao;
    u32 *ebo;

    f32 *vertices;
    int vertices_len;
    short vertex_stride;
    u32 *indices;
    int indices_len;
    short index_stride;
    f32 *uv_vertices;
    int uv_vertices_len;

    int shader_position;
    f32 *positional_vertices;
    int positional_vertices_len;
    Array *entity_ids;

    /** Core render item data */
    vec2 size;
    vec4 color;
    vec3 rotation;
    mat4x4 model;
    float opacity;

    /** Physics */
    short is_collided;
    short is_static_object;

    /** Texture */
    Texture_Data *texture_data;

    /** Sprite Sheet */
    Sprite_Sheet_Data *sprite_sheet_data;

    /** Animation */
    Map *animations;

} Render_Item;

/** TODO: Organize */
Render_Item *init_render_item(Render_Item *render_item, float *pos, vec2 size, vec3 rotation, vec4 color, int offset[2], float points[12], float opacity);
Render_Item *get_render_item(short make_new, RENDER_ITEM_TYPE type, int shader_position, char *texture_file_path);
Render_Item *add_render_item(Render_Item *render_item);
void append_item_to_render_item(Render_Item *render_item, void *entity);
void remove_item_from_render_item(Render_Item *render_item, void *entity);
void update_render_item(void *entity);

float *get_uvs(int *offset, int *sprite_sheet_count, int *sprite_size);
Render_Item *create_quad(Render_Item *render_item, vec3 pos, int vbo_pos, vec2 size, int is_vertical, int offset[2]);
Render_Item *create_cube(Render_Item *render_item);
Render_Item *create_line(Render_Item *render_item, float vertices[6]);
Vertex **create_circle_vertex_array(float x, float y, float z, float diameter, int vertices, vec4 color);
Render_Item *create_circle(Render_Item *render_item, vec3 pos, int vbo_pos, vec2 size, int vertices);
Render_Item *create_rect(Render_Item *render_item, vec3 pos, int vbo_pos, vec2 size);
Vertex *create_rect_vertices(float x, float y, float z, vec2 size);
Vertex *create_quad_vertices_from_points(float x, float y, float z, float points[12], float *uvs);

Vertex create_vertex(vec3 pos, vec2 uvs, vec4 color, vec3 normal);
Vertex *create_quad_vertices(float x, float y, float z, vec2 size, float *uvs);
Vertex *create_vertical_quad_vertices(float x, float y, float z, vec2 size, float *uvs);

Render_Item *create_render_item(RENDER_ITEM_TYPE type);
Render_Item *add_shader(Render_Item *render_item, int shader_position);
Render_Item *add_texture(Render_Item *render_item, char *file_path);
Render_Item *add_sprite_sheet_data(Render_Item *render_item, int sprite_size[2], int sprite_sheet_size[2]);

void bind_render_item_data(Render_Item *render_item);

mat4x4 *scale_model(mat4x4 *model, float *size, short should_ignore_camera);
mat4x4 *rotate_model(mat4x4 *model, float *rotation);

Render_Item *add_animation(Render_Item *render_item, Animation *animation);

void print_render_item(Render_Item *render_item);
