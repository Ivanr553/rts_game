#include <linmath.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stb/stb_image.h>

#include "render_item.h"

#include "../../entity/entity.h"
#include "../../global.h"

#include "../opengl_objects/opengl_objects.h"

long count = 1;

float *get_uvs(int offset[2], int *sprite_sheet_count, int *sprite_size)
{
    /** left, right, bottom, top */
    float *uvs = calloc(4, sizeof(float));

    if (!offset || !sprite_sheet_count || !sprite_size)
    {
        uvs[0] = 0;
        uvs[1] = 1;
        uvs[2] = 0;
        uvs[3] = 1;
        return uvs;
    }

    float x_offset = (offset[0] - 1) * sprite_size[0];
    float y_offset = (offset[1] - 1) * sprite_size[1];

    int sprite_sheet_width = sprite_sheet_count[0] * sprite_size[0];
    int sprite_sheet_height = sprite_sheet_count[1] * sprite_size[1];

    float left = x_offset / (sprite_sheet_width);
    float right = (x_offset + sprite_size[0]) / (sprite_sheet_width);
    float bottom = ((sprite_sheet_height) - (y_offset + sprite_size[1])) / sprite_sheet_height;
    float top = ((sprite_sheet_height) - (y_offset)) / sprite_sheet_height;

    uvs[0] = left;
    uvs[1] = right;
    uvs[2] = bottom;
    uvs[3] = top;

    return uvs;
}

Vertex create_vertex(vec3 pos, vec2 uvs, vec4 color, vec3 normal)
{
    Vertex vertex;

    vertex.pos[0] = pos[0];
    vertex.pos[1] = pos[1];
    vertex.pos[2] = pos[2];

    if (uvs)
    {
        vertex.uvs[0] = uvs[0];
        vertex.uvs[1] = uvs[1];
    }

    if (color)
    {
        vertex.color[0] = color[0];
        vertex.color[1] = color[1];
        vertex.color[2] = color[2];
        vertex.color[3] = color[3];
    }

    if (normal)
    {
        vertex.normal[0] = normal[0];
        vertex.normal[1] = normal[1];
        vertex.normal[2] = normal[2];
    }

    return vertex;
}

Vertex *create_rect_vertices(float x, float y, float z, vec2 size)
{
    Vertex *vertices = calloc(4, sizeof(Vertex));

    f32 left_pos = x + (-(size[0] / 2));
    f32 right_pos = x + (size[0] / 2);
    f32 bottom_pos = y + (-(size[1] / 2));
    f32 top_pos = y + (size[1] / 2);

    Vertex vertex_bottom_left = create_vertex((vec3){left_pos, bottom_pos, z}, NULL, NULL, NULL);
    Vertex vertex_bottom_right = create_vertex((vec3){right_pos, bottom_pos, z}, NULL, NULL, NULL);
    Vertex vertex_top_right = create_vertex((vec3){right_pos, top_pos, z}, NULL, NULL, NULL);
    Vertex vertex_top_left = create_vertex((vec3){left_pos, top_pos, z}, NULL, NULL, NULL);

    vertices[0] = vertex_bottom_left;
    vertices[1] = vertex_bottom_right;
    vertices[2] = vertex_top_right;
    vertices[3] = vertex_top_left;

    return vertices;
};

Vertex *create_quad_vertices_from_points(float x, float y, float z, float points[12], float *uvs)
{
    Vertex *vertices = calloc(4, sizeof(Vertex));

    vec4 color = {0, 0, 0, 0};
    vec3 normal = {0, 0, 1};

    float left = uvs[0];
    float right = uvs[1];
    float bottom = uvs[2];
    float top = uvs[3];

    Vertex vertex_bottom_left = create_vertex((vec3){points[0], points[1], points[2]}, (vec2){left, bottom}, color, normal);
    Vertex vertex_bottom_right = create_vertex((vec3){points[3], points[4], points[5]}, (vec2){right, bottom}, color, normal);
    Vertex vertex_top_right = create_vertex((vec3){points[6], points[7], points[8]}, (vec2){right, top}, color, normal);
    Vertex vertex_top_left = create_vertex((vec3){points[9], points[10], points[11]}, (vec2){left, top}, color, normal);

    vertices[0] = vertex_bottom_left;
    vertices[1] = vertex_bottom_right;
    vertices[2] = vertex_top_right;
    vertices[3] = vertex_top_left;

    return vertices;
};

Vertex *create_quad_vertices(float x, float y, float z, vec2 size, float *uvs)
{
    Vertex *vertices = calloc(4, sizeof(Vertex));

    f32 left_pos = x + (-(size[0] / 2));
    f32 right_pos = x + (size[0] / 2);
    f32 bottom_pos = y + (-(size[1] / 2));
    f32 top_pos = y + (size[1] / 2);

    vec4 color = {0, 0, 0, 0};
    vec3 normal = {0, 0, 1};

    float left = uvs[0];
    float right = uvs[1];
    float bottom = uvs[2];
    float top = uvs[3];

    Vertex vertex_bottom_left = create_vertex((vec3){left_pos, bottom_pos, z}, (vec2){left, bottom}, color, normal);
    Vertex vertex_bottom_right = create_vertex((vec3){right_pos, bottom_pos, z}, (vec2){right, bottom}, color, normal);
    Vertex vertex_top_right = create_vertex((vec3){right_pos, top_pos, z}, (vec2){right, top}, color, normal);
    Vertex vertex_top_left = create_vertex((vec3){left_pos, top_pos, z}, (vec2){left, top}, color, normal);

    vertices[0] = vertex_bottom_left;
    vertices[1] = vertex_bottom_right;
    vertices[2] = vertex_top_right;
    vertices[3] = vertex_top_left;

    return vertices;
};

Vertex *create_vertical_quad_vertices(float x, float y, float z, vec2 size, float *uvs)
{
    Vertex *vertices = calloc(4, sizeof(Vertex));

    f32 left_pos = x + (-(size[0] / 2));
    f32 right_pos = x + (size[0] / 2);
    f32 bottom_pos = z + (-(size[1] / 2));
    f32 top_pos = z + (size[1] / 2);

    vec4 color = {0, 0, 0, 0};
    vec3 normal = {0, 0, 1};

    float left = uvs[0];
    float right = uvs[1];
    float bottom = uvs[2];
    float top = uvs[3];

    Vertex vertex_bottom_left = create_vertex((vec3){left_pos, y, bottom_pos}, (vec2){left, bottom}, color, normal);
    Vertex vertex_bottom_right = create_vertex((vec3){right_pos, y, bottom_pos}, (vec2){right, bottom}, color, normal);
    Vertex vertex_top_right = create_vertex((vec3){right_pos, y + 0.6, top_pos - 0.2}, (vec2){right, top}, color, normal);
    Vertex vertex_top_left = create_vertex((vec3){left_pos, y + 0.6, top_pos - 0.2}, (vec2){left, top}, color, normal);

    vertices[0] = vertex_bottom_left;
    vertices[1] = vertex_bottom_right;
    vertices[2] = vertex_top_right;
    vertices[3] = vertex_top_left;

    return vertices;
};

Vertex **create_circle_vertex_array(float x, float y, float z, float diameter, int vertices, vec4 color)
{
    Vertex **vertex_array = calloc(vertices, sizeof(Vertex));

    float two_pi = 2 * M_PI;

    for (int i = 0; i < vertices; i++)
    {
        float theta = 360 / vertices;
        float theta_r = theta * M_PI / 180;
        vec3 pos = {
            x + ((diameter / 2) * cos(i * two_pi / vertices)),
            y + ((diameter / 2) * sin(i * two_pi / vertices)),
            z};

        vec3 normal = {0, 0, 1};
        Vertex *vertex = calloc(1, sizeof(Vertex));

        vertex->pos[0] = pos[0];
        vertex->pos[1] = pos[1];
        vertex->pos[2] = pos[2];

        vertex_array[i] = vertex;
    }

    return vertex_array;
};

Render_Item *create_circle(Render_Item *render_item, vec3 pos, int vbo_pos, vec2 size, int vertices)
{
    Vertex **vertex_array = create_circle_vertex_array(pos[0], pos[1], pos[2], size[0], vertices, render_item->color);

    for (int i = 0; i < vertices; i++)
    {
        Vertex *vertex = vertex_array[i];
        int vertex_offset = vbo_pos * vertices;
        int b = i * render_item->vertex_stride;
        int c = 0;

        /** POS */
        render_item->vertices[vertex_offset + b + c++] = vertex->pos[0];
        render_item->vertices[vertex_offset + b + c++] = vertex->pos[1];
        render_item->vertices[vertex_offset + b + c++] = vertex->pos[2];

        free(vertex);
    }

    free(vertex_array);
    return render_item;
}

Render_Item *create_rect(Render_Item *render_item, vec3 pos, int vbo_pos, vec2 size)
{
    Vertex *quad_vertices = create_rect_vertices(pos[0], pos[1], pos[2], size);

    int positional_vertices_index = 0;
    int quad_vertex_amount = 4;
    for (int i = 0; i < quad_vertex_amount; i++)
    {
        Vertex vertex = quad_vertices[i];
        int vertex_offset = vbo_pos * (quad_vertex_amount * render_item->vertex_stride);
        int b = i * render_item->vertex_stride;
        int c = 0;

        /** POS */
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[1];
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[2];
    }

    // int default_indices[6] = {0, 1, 2, 2, 3, 0};
    // for (int i = 0; i < DEFAULT_QUAD_INDICES_LEN; i++)
    // {
    //     int index_offset = vbo_pos * DEFAULT_QUAD_INDICES_LEN;
    //     render_item->indices[index_offset + i] = default_indices[i] + (vbo_pos * quad_vertex_amount);
    // }

    free(quad_vertices);

    return render_item;
};

Render_Item *create_quad_from_points(Render_Item *render_item, vec3 pos, int vbo_pos, vec2 size, int is_vertical, int offset[2], float points[12])
{
    float *uvs = get_uvs(offset, render_item->sprite_sheet_data->sprite_sheet_size, render_item->sprite_sheet_data->sprite_size);

    Vertex *quad_vertices;
    if (!points)
    {
        quad_vertices = create_quad_vertices(pos[0], pos[1], pos[2], size, uvs);
    }
    else
    {
        quad_vertices = create_quad_vertices_from_points(pos[0], pos[1], pos[2], points, uvs);
    }

    int positional_vertices_index = 0;
    int quad_vertex_amount = 4;
    for (int i = 0; i < quad_vertex_amount; i++)
    {
        Vertex vertex = quad_vertices[i];
        int vertex_offset = vbo_pos * (quad_vertex_amount * render_item->vertex_stride);
        int b = i * sizeof(vertex) / quad_vertex_amount;
        int c = 0;

        /** POS */
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[1];
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[2];

        render_item->positional_vertices[positional_vertices_index++] = vertex.pos[0];
        render_item->positional_vertices[positional_vertices_index++] = vertex.pos[1];
        render_item->positional_vertices[positional_vertices_index++] = vertex.pos[2];

        /** UVS */
        render_item->vertices[vertex_offset + b + c++] = vertex.uvs[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.uvs[1];

        /** COLOR */
        render_item->vertices[vertex_offset + b + c++] = vertex.color[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.color[1];
        render_item->vertices[vertex_offset + b + c++] = vertex.color[2];
        render_item->vertices[vertex_offset + b + c++] = vertex.color[3];

        /** NORMAL */
        render_item->vertices[vertex_offset + b + c++] = vertex.normal[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.normal[1];
        render_item->vertices[vertex_offset + b + c++] = vertex.normal[2];
    }

    int default_indices[6] = {0, 1, 2, 2, 3, 0};
    for (int i = 0; i < DEFAULT_QUAD_INDICES_LEN; i++)
    {
        int index_offset = vbo_pos * DEFAULT_QUAD_INDICES_LEN;
        render_item->indices[index_offset + i] = default_indices[i] + (vbo_pos * quad_vertex_amount);
    }

    free(quad_vertices);
    free(uvs);

    return render_item;
}

Render_Item *create_quad(Render_Item *render_item, vec3 pos, int vbo_pos, vec2 size, int is_vertical, int offset[2])
{
    float *uvs = get_uvs(offset, render_item->sprite_sheet_data->sprite_sheet_size, render_item->sprite_sheet_data->sprite_size);

    Vertex *quad_vertices;
    if (is_vertical)
    {
        quad_vertices = create_vertical_quad_vertices(pos[0], pos[1], pos[2], size, uvs);
    }
    else
    {
        quad_vertices = create_quad_vertices(pos[0], pos[1], pos[2], size, uvs);
    }

    int positional_vertices_index = 0;
    int quad_vertex_amount = 4;
    for (int i = 0; i < quad_vertex_amount; i++)
    {
        Vertex vertex = quad_vertices[i];
        int vertex_offset = vbo_pos * (quad_vertex_amount * render_item->vertex_stride);
        int b = i * sizeof(vertex) / quad_vertex_amount;
        int c = 0;

        /** POS */
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[1];
        render_item->vertices[vertex_offset + b + c++] = vertex.pos[2];

        render_item->positional_vertices[positional_vertices_index++] = vertex.pos[0];
        render_item->positional_vertices[positional_vertices_index++] = vertex.pos[1];
        render_item->positional_vertices[positional_vertices_index++] = vertex.pos[2];

        /** UVS */
        render_item->vertices[vertex_offset + b + c++] = vertex.uvs[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.uvs[1];

        /** COLOR */
        render_item->vertices[vertex_offset + b + c++] = vertex.color[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.color[1];
        render_item->vertices[vertex_offset + b + c++] = vertex.color[2];
        render_item->vertices[vertex_offset + b + c++] = vertex.color[3];

        /** NORMAL */
        render_item->vertices[vertex_offset + b + c++] = vertex.normal[0];
        render_item->vertices[vertex_offset + b + c++] = vertex.normal[1];
        render_item->vertices[vertex_offset + b + c++] = vertex.normal[2];
    }

    int default_indices[6] = {0, 1, 2, 2, 3, 0};
    for (int i = 0; i < DEFAULT_QUAD_INDICES_LEN; i++)
    {
        int index_offset = vbo_pos * DEFAULT_QUAD_INDICES_LEN;
        render_item->indices[index_offset + i] = default_indices[i] + (vbo_pos * quad_vertex_amount);
    }

    free(quad_vertices);
    free(uvs);

    return render_item;
};

Render_Item *create_cube(Render_Item *render_item)
{
    f32 vertices[] =
        {//  COORDINATES  //
         -0.1f, -0.1f, 0.1f,
         -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, 0.1f,
         -0.1f, 0.1f, 0.1f,
         -0.1f, 0.1f, -0.1f,
         0.1f, 0.1f, -0.1f,
         0.1f, 0.1f, 0.1f};

    u32 indices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 4, 7,
            0, 7, 3,
            3, 7, 6,
            3, 6, 2,
            2, 6, 5,
            2, 5, 1,
            1, 5, 4,
            1, 4, 0,
            4, 5, 6,
            4, 6, 7};

    f32 *new_vertices = calloc(24, sizeof(f32));
    for (int i = 0; i < 24; i++)
    {
        new_vertices[i] = vertices[i];
    }
    render_item->vertices = new_vertices;
    render_item->vertices_len = 24;

    u32 *new_indices = calloc(36, sizeof(u32));
    for (int i = 0; i < 36; i++)
    {
        new_indices[i] = indices[i];
    }
    render_item->indices = new_indices;
    render_item->indices_len = 36;

    render_item->vertex_stride = 3;
    render_item->index_stride = 12;

    return render_item;
};

Render_Item *create_line(Render_Item *render_item, float *vertices)
{
    f32 *new_vertices = calloc(6, sizeof(float));

    render_item->vertices = new_vertices;
    render_item->vertices_len = 6;

    render_item->positional_vertices = new_vertices;
    render_item->positional_vertices_len = 6;

    render_item->indices_len = 0;

    render_item->vertex_stride = 3;

    return render_item;
}

/* ================= RENDER ITEM ================= */
Render_Item *create_render_item(RENDER_ITEM_TYPE type)
{
    Render_Item *render_item = calloc(1, sizeof(Render_Item));
    render_item->id = count++;
    render_item->type = type;

    return render_item;
};

Render_Item *add_shader(Render_Item *render_item, int shader_position)
{
    if (!render_item)
    {
        printf("No render item passed to add_shader\n");
        return NULL;
    }

    render_item->shader_position = shader_position;

    return render_item;
};

Render_Item *add_texture(Render_Item *render_item, char *file_path)
{
    if (!render_item)
    {
        printf("No render item passed to add_texture\n");
        return NULL;
    }

    Texture_Data *texture_data_in_map = get_value_from_map(global.render.texture_map, file_path);

    if (!texture_data_in_map)
    {
        printf("That's awkward, we really should have that texture data\n");
        exit(1);
    }

    render_item->texture_data = texture_data_in_map;

    return render_item;
};

Render_Item *add_sprite_sheet_data(Render_Item *render_item, int sprite_size[2], int sprite_sheet_size[2])
{
    if (!render_item)
    {
        printf("No render item passed to add_sprite_sheet_data\n");
        return NULL;
    }

    render_item->sprite_sheet_data = calloc(1, sizeof(Sprite_Sheet_Data));

    render_item->sprite_sheet_data->sprite_size[0] = sprite_size[0];
    render_item->sprite_sheet_data->sprite_size[1] = sprite_size[1];

    render_item->sprite_sheet_data->sprite_sheet_size[0] = sprite_sheet_size[0];
    render_item->sprite_sheet_data->sprite_sheet_size[1] = sprite_sheet_size[1];

    return render_item;
};

Render_Item *add_animation(Render_Item *render_item, Animation *animation)
{
    if (!render_item)
    {
        printf("No render item passed to add_animation\n");
        return NULL;
    }

    if (!render_item->animations)
    {
        printf("No animation map set, creating the animation map\n");
        render_item->animations = create_map(DEFAULT_ANIMATION_MAP_SIZE, 0);
    }

    if (!set_value_in_map(render_item->animations, animation->animation_name, animation))
    {
        printf("Unable to add animation to animation map\n");
        return NULL;
    }

    // printf("Added animation to animation map\n");
    return render_item;
};

Render_Item *init_render_item(Render_Item *render_item, float *pos, vec2 size, vec3 rotation, vec4 color, int offset[2], float points[12], float opacity)
{
    // printf("Initializing item\n");
    render_item->opacity = opacity;

    if (rotation)
    {
        render_item->rotation[0] = rotation[0];
        render_item->rotation[1] = rotation[1];
        render_item->rotation[2] = rotation[2];
    }

    if (!color)
    {
        vec4 _color = {255, 255, 255, 255};

        render_item->color[0] = _color[0];
        render_item->color[1] = _color[1];
        render_item->color[2] = _color[2];
        render_item->color[3] = _color[3];
    }
    else
    {
        render_item->color[0] = color[0];
        render_item->color[1] = color[1];
        render_item->color[2] = color[2];
        render_item->color[3] = color[3];
    }

    /** TODO: will have to update this logic to support cubes and such (if ever needed) */
    switch (render_item->type)
    {
    case RENDER_ITEM_QUAD:
        render_item->vertices = calloc(DEFAULT_QUAD_VERTICES_LEN, sizeof(f32));
        render_item->vertices_len = DEFAULT_QUAD_VERTICES_LEN;
        render_item->positional_vertices = calloc(DEFAULT_QUAD_STRIDE, sizeof(f32));
        render_item->positional_vertices_len = DEFAULT_QUAD_STRIDE;
        render_item->vertex_stride = DEFAULT_QUAD_STRIDE;
        render_item->indices = calloc(DEFAULT_QUAD_INDICES_LEN, sizeof(u32));
        render_item->indices_len = DEFAULT_QUAD_INDICES_LEN;
        render_item->index_stride = DEFAULT_QUAD_INDICES_LEN;
        create_quad(render_item, pos, 0, size, 0, offset);
        break;
    case RENDER_ITEM_VERTICAL_QUAD:
        render_item->vertices = calloc(DEFAULT_QUAD_VERTICES_LEN, sizeof(f32));
        render_item->vertices_len = DEFAULT_QUAD_VERTICES_LEN;
        render_item->positional_vertices = calloc(DEFAULT_QUAD_STRIDE, sizeof(f32));
        render_item->positional_vertices_len = DEFAULT_QUAD_STRIDE;
        render_item->vertex_stride = DEFAULT_QUAD_STRIDE;
        render_item->indices = calloc(DEFAULT_QUAD_INDICES_LEN, sizeof(u32));
        render_item->indices_len = DEFAULT_QUAD_INDICES_LEN;
        render_item->index_stride = DEFAULT_QUAD_INDICES_LEN;
        create_quad(render_item, pos, 0, size, 1, offset);
        break;

    case RENDER_ITEM_CIRCLE:
        render_item->vertices = calloc(DEFAULT_CIRCLE_VERTICES_AMOUNT * DEFAULT_POS_ONLY_VERTEX_STRIDE, sizeof(f32));
        render_item->vertices_len = DEFAULT_CIRCLE_VERTICES_AMOUNT * DEFAULT_POS_ONLY_VERTEX_STRIDE;
        render_item->vertex_stride = DEFAULT_POS_ONLY_VERTEX_STRIDE;
        create_circle(render_item, pos, 0, size, DEFAULT_CIRCLE_VERTICES_AMOUNT);
        break;

    case RENDER_ITEM_RECT:
        render_item->vertices = calloc(4 * DEFAULT_POS_ONLY_VERTEX_STRIDE, sizeof(f32));
        render_item->vertices_len = 4 * DEFAULT_POS_ONLY_VERTEX_STRIDE;
        render_item->vertex_stride = DEFAULT_POS_ONLY_VERTEX_STRIDE;
        create_rect(render_item, pos, 0, size);
        break;

    case RENDER_ITEM_QUAD_WITH_POINTS:
        render_item->vertices = calloc(DEFAULT_QUAD_VERTICES_LEN, sizeof(f32));
        render_item->vertices_len = DEFAULT_QUAD_VERTICES_LEN;
        render_item->positional_vertices = calloc(DEFAULT_QUAD_STRIDE, sizeof(f32));
        render_item->positional_vertices_len = DEFAULT_QUAD_STRIDE;
        render_item->vertex_stride = DEFAULT_QUAD_STRIDE;
        render_item->indices = calloc(DEFAULT_QUAD_INDICES_LEN, sizeof(u32));
        render_item->indices_len = DEFAULT_QUAD_INDICES_LEN;
        render_item->index_stride = DEFAULT_QUAD_INDICES_LEN;
        create_quad_from_points(render_item, pos, 0, size, 0, offset, points);
        break;

    default:
        printf("Unable to map render_item to type: %d\n", render_item->type);
        return NULL;
    }

    return render_item;
};

void bind_render_item_data(Render_Item *render_item)
{
    render_item->vao = calloc(1, sizeof(GLuint));
    render_item->vbo = calloc(1, sizeof(GLuint));
    render_item->ebo = calloc(1, sizeof(GLuint));

    // printf("Added vertices and indices\n");
    generate_vao(render_item);
    // printf("Generated vao\n");
    bind_vao(render_item->vao);
    // printf("Bound vao\n");

    generate_dynamic_vbo(render_item->vertices, render_item->vertices_len, render_item->vbo);
    // printf("Generated vbo\n");
    generate_ebo(render_item->indices, render_item->indices_len, render_item->ebo);
    // printf("Generated ebo\n");

    link_attributes(render_item);

    // Cleanup
    unbind_vao(render_item);
    unbind_vbo(render_item);
    if (render_item->indices_len != 0)
    {
        unbind_ebo(render_item);
    }
    // printf("Finished cleanup\n");
}

mat4x4 *scale_model(mat4x4 *model, float *size, short should_ignore_camera)
{
    mat4x4 scale = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    mat4x4_identity(*model);
    mat4x4_mul(*model, *model, scale);

    return model;
}

mat4x4 *rotate_model(mat4x4 *model, float *rotation)
{
    float rotationX = rotation[0];
    float rotationY = rotation[1];
    float rotationZ = rotation[2];

    mat4x4_rotate(*model, *model, 1, 0, 0, rotationX);
    mat4x4_rotate(*model, *model, 0, 1, 0, rotationY);
    mat4x4_rotate(*model, *model, 0, 0, 1, rotationZ);

    return model;
}

void print_render_item(Render_Item *render_item)
{
    printf("\n");
    printf("=== RENDER ITEM INFO ===\n");
    printf(" - Vertices: %d - \n ", render_item->vertices_len);
    f32 *print_vertices = render_item->vertices;
    for (int i = 0; i < render_item->vertices_len; i++)
    {
        printf("%f, ", print_vertices[i]);
        if ((i + 1) % render_item->vertex_stride == 0)
        {
            printf("\n ");
        }
    }
    printf("\n - Vertices End - \n");

    printf(" - Indices: %d - \n ", render_item->indices_len);
    u32 *print_indices = render_item->indices;
    for (int i = 0; i < render_item->indices_len; i++)
    {
        printf("%d, ", print_indices[i]);
    }
    printf("\n - Indices End - \n");
    printf(" - OpenGL Object Values - \n");
    printf(" VAO: %d\n", *render_item->vao);
    printf(" VBO: %d\n", *render_item->vbo);
    printf(" EBO: %d\n", *render_item->ebo);
    printf(" - OpenGL Object Values End- \n");

    printf("=== RENDER ITEM INFO END ===\n\n");
}

Render_Item *add_render_item(Render_Item *render_item)
{
    Shader_Group shader_group = global.render.shader_groups[render_item->shader_position];
    Texture_Group texture_group = shader_group.texture_groups[render_item->texture_data->texture_location];
    render_item->render_item_index = texture_group.render_items->len;

    printf("Adding render item. Shader: %d | Texture: %d | Length: %zd\n", render_item->shader_position, render_item->texture_data->texture_location, texture_group.render_items->len);
    return append_array(texture_group.render_items, render_item);
}

Render_Item *get_render_item(short make_new, RENDER_ITEM_TYPE type, int shader_position, char *texture_file_path)
{
    Texture_Data texture_data = *((Texture_Data *)get_value_from_map(global.render.texture_map, texture_file_path));
    short texture_location = texture_data.texture_location;

    Array *render_items = global.render.shader_groups[shader_position].texture_groups[texture_location].render_items;

    if (render_items->len == 0 || make_new)
    {
        printf("Creating new render item for texture: %s\n", texture_file_path);
        Render_Item *render_item = calloc(1, sizeof(Render_Item));
        render_item->id = count++;
        render_item->entity_ids = create_array(100, sizeof(long *));
        render_item->shader_position = shader_position;
        render_item->texture_data = get_value_from_map(global.render.texture_map, texture_file_path);
        render_item->type = type;
        render_item->render_item_index = render_items->len;
        // printf("Creating render item with type: %d and at index: %d\n", type, render_item->render_item_index);
        render_item = append_array(render_items, render_item);
        printf("Render items len: %zd\n", render_items->len);

        return render_item;
    }
    else
    {
        // printf("Getting render item from array\n");
        return get_item_from_array(render_items, 0);
    }
}

void append_item_to_render_item(Render_Item *render_item, void *entity)
{
    Entity *_entity = entity;
    _entity->render_item = render_item;
    append_array(render_item->entity_ids, &_entity->id);
    _entity->vbo_pos = render_item->entity_ids->len - 1;

    if (render_item->entity_ids->len > 1)
    {

        bind_vao(render_item->vao);
        bind_vbo(render_item->vbo);

        switch (render_item->type)
        {
        case RENDER_ITEM_QUAD:
            render_item->vertices_len = DEFAULT_QUAD_VERTICES_LEN * render_item->entity_ids->len;
            render_item->vertices = realloc(render_item->vertices, render_item->vertices_len * sizeof(f32));
            render_item->positional_vertices_len = DEFAULT_QUAD_STRIDE * render_item->entity_ids->len;
            render_item->positional_vertices = realloc(render_item->positional_vertices, render_item->positional_vertices_len * sizeof(f32));
            render_item->indices_len = DEFAULT_QUAD_INDICES_LEN * render_item->entity_ids->len;
            render_item->indices = realloc(render_item->indices, render_item->indices_len * sizeof(u32));
            create_quad(render_item, _entity->pos, _entity->vbo_pos, _entity->size, 0, _entity->offset);
            break;

        case RENDER_ITEM_VERTICAL_QUAD:
            render_item->vertices_len = DEFAULT_QUAD_VERTICES_LEN * render_item->entity_ids->len;
            render_item->vertices = realloc(render_item->vertices, render_item->vertices_len * sizeof(f32));
            render_item->positional_vertices_len = DEFAULT_QUAD_STRIDE * render_item->entity_ids->len;
            render_item->positional_vertices = realloc(render_item->positional_vertices, render_item->positional_vertices_len * sizeof(f32));
            render_item->indices_len = DEFAULT_QUAD_INDICES_LEN * render_item->entity_ids->len;
            render_item->indices = realloc(render_item->indices, render_item->indices_len * sizeof(u32));
            create_quad(render_item, _entity->pos, _entity->vbo_pos, _entity->size, 1, _entity->offset);
            break;

        case RENDER_ITEM_CIRCLE:
            render_item->vertices = calloc(DEFAULT_CIRCLE_VERTICES_AMOUNT * DEFAULT_POS_ONLY_VERTEX_STRIDE * render_item->entity_ids->len, sizeof(f32));
            render_item->vertices_len = DEFAULT_CIRCLE_VERTICES_AMOUNT * DEFAULT_POS_ONLY_VERTEX_STRIDE * render_item->entity_ids->len;
            create_circle(render_item, _entity->pos, _entity->vbo_pos, _entity->size, DEFAULT_CIRCLE_VERTICES_AMOUNT);
            break;

        case RENDER_ITEM_QUAD_WITH_POINTS:
            render_item->vertices_len = DEFAULT_QUAD_VERTICES_LEN * render_item->entity_ids->len;
            render_item->vertices = realloc(render_item->vertices, render_item->vertices_len * sizeof(f32));
            render_item->positional_vertices_len = DEFAULT_QUAD_STRIDE * render_item->entity_ids->len;
            render_item->positional_vertices = realloc(render_item->positional_vertices, render_item->positional_vertices_len * sizeof(f32));
            render_item->indices_len = DEFAULT_QUAD_INDICES_LEN * render_item->entity_ids->len;
            render_item->indices = realloc(render_item->indices, render_item->indices_len * sizeof(u32));
            create_quad_from_points(render_item, _entity->pos, 0, _entity->size, 0, _entity->offset, _entity->points);
            break;

        default:
            printf("Unable to map render_item to type: %d\n", render_item->type);
            return;
        }

        /** update buffer sizes */
        glBufferData(GL_ARRAY_BUFFER, render_item->vertices_len * sizeof(f32), render_item->vertices, GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_item->indices_len * sizeof(u32), render_item->indices, GL_STATIC_DRAW);

        unbind_vao(render_item);
        unbind_vbo(render_item);
    }
}

void remove_item_from_render_item(Render_Item *render_item, void *entity)
{
    Entity *_entity = entity;
    _entity->render_item = render_item;
}

void update_render_item(void *entity)
{
    Entity *_entity = entity;
    Render_Item *render_item = _entity->render_item;

    switch (render_item->type)
    {
    case RENDER_ITEM_QUAD:
        create_quad(render_item, _entity->pos, _entity->vbo_pos, _entity->size, 0, _entity->offset);
        break;
    case RENDER_ITEM_VERTICAL_QUAD:
        create_quad(render_item, _entity->pos, _entity->vbo_pos, _entity->size, 1, _entity->offset);
        break;
    case RENDER_ITEM_RECT:
        create_rect(render_item, _entity->pos, _entity->vbo_pos, _entity->size);
        break;
    case RENDER_ITEM_CIRCLE:
        create_circle(render_item, _entity->pos, _entity->vbo_pos, _entity->size, DEFAULT_CIRCLE_VERTICES_AMOUNT);
        break;
    case RENDER_ITEM_QUAD_WITH_POINTS:
        create_quad_from_points(render_item, _entity->pos, 0, _entity->size, 0, _entity->offset, _entity->points);
        break;

    default:
        printf("Unable to map render_item to type: %d\n", render_item->type);
        return;
    }

    render_item->updated = 1;
}