#pragma once

#include <SDL2/SDL.h>
#include <linmath.h>
#include <glad/glad.h>

#include "../types.h"
#include "../io/io.h"
#include "../../generic/generic.h"

#include "opengl_objects/opengl_objects.h"
#include "render_fonts/render_fonts.h"
#include "render_item/render_item.h"
#include "util/render_util.h"
#include "../entity/entity.h"

#define EMPTY_TEXTURE "EMPTY_TEXTURE"

#define SUPPORTED_SHADER_COUNT 3
#define SUPPORTED_TEXTURE_COUNT 12
#define INITIAL_ENTITY_ARRAY_COUNT 100

#define SHADER_DEFAULT 0
#define SHADER_LIGHT 1
#define SHADER_COLOR 2

typedef struct shader_data
{
    char *vert_path;
    char *frag_path;
} Shader_Data;

typedef struct texture_group
{
    GLuint *texture_id;
    Array *render_items;
} Texture_Group;

typedef struct shader_group
{
    int shader_id;
    Texture_Group *texture_groups;
} Shader_Group;

typedef struct render_items
{
    Render_Item *list;
    int next_item_index;
    int size;
} Render_Items;

typedef struct render_state
{
    int tick;
    SDL_Window *window;
    f32 width;
    f32 height;
    float scale;

    /** V2 */
    Map *texture_map;
    GLuint textures[SUPPORTED_TEXTURE_COUNT];
    Shader_Group *shader_groups;

    /** Entities */
    Array *entities;
    Array *non_static_entities;
    Array *ui_entities;
    Entity_Map entity_map;

} Render_State;

void render_init(void);
void render_begin(void);
void render_end(void);

void use_shader(Render_Item *item, int shader_id);
float *get_ray_from_line(vec3 start, vec3 end);

void render(void);