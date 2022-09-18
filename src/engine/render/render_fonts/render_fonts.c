#include <string.h>

#include "render_fonts.h"
#include "../../global.h"

void render_text_in_world(char *c, vec3 pos)
{

    float overlap_ratio = 0.8;
    for (int i = 0; i < strlen(c); i++)
    {
        Render_Item *character_render_item = create_render_item(RENDER_ITEM_QUAD);

        int char_int = c[i];
        int x = (char_int - 33) % 11;
        int y = floor((char_int - 33) / 11);

        int offset[2] = {x + 1, y + 1};
        int sprite_sheet_size[2] = {11, 9};
        int sprite_size[2] = {7, 7};
        vec2 size = {0.25, 0.25};

        vec3 new_pos = {
            pos[0] + (i * overlap_ratio),
            pos[1],
            pos[2]};

        add_shader(character_render_item, 0);
        add_sprite_sheet_data(character_render_item, offset, sprite_size, sprite_sheet_size);
        init_render_item(character_render_item, new_pos, size, NULL, NULL);
        bind_render_item_data(character_render_item);
        add_texture(character_render_item, "assets/UI/Text.png");

        // append_array(global.render.render_items, character_render_item);
    }
};

Render_Item *render_text_on_screen(Render_Item *render_item, char *c, int max_str_len, vec3 pos, vec2 size)
{
    int length = strlen(c);

    f32 *vertices;
    u32 *indices;

    for (int i = 0; i < length; i++)
    {
        if (i == 0)
        {
            render_item->vertices_len = max_str_len * DEFAULT_QUAD_VERTICES_LEN;
            vertices = calloc(render_item->vertices_len, sizeof(f32));
            render_item->indices_len = max_str_len * DEFAULT_QUAD_INDICES_LEN;
            indices = calloc(render_item->indices_len, sizeof(u32));
            render_item->vertex_stride = DEFAULT_QUAD_STRIDE;
            render_item->index_stride = DEFAULT_QUAD_INDICES_LEN;
        }

        int char_int = c[i];
        int x = ((char_int - 33) % 11);
        int y = floor((char_int - 33) / 11);

        int offset[2] = {x + 1, y + 1};
        int sprite_sheet_count[2] = {11, 9};
        int sprite_size[2] = {1, 1};
        float overlap_ratio = 0.8 * size[0];

        float *uvs = get_uvs(offset, sprite_sheet_count, sprite_size);
        Vertex *quad_vertices = create_quad_vertices(pos[0] + (i * overlap_ratio), pos[1], 0, size, uvs);

        for (int j = 0; j < 4; j++)
        {
            Vertex vertex = quad_vertices[j];
            int b = (i * DEFAULT_QUAD_VERTICES_LEN) + j * sizeof(vertex) / 4;
            int c = 0;

            /** POS */
            vertices[b + c++] = vertex.pos[0];
            vertices[b + c++] = vertex.pos[1];
            vertices[b + c++] = vertex.pos[2];

            /** UVS */
            vertices[b + c++] = vertex.uvs[0];
            vertices[b + c++] = vertex.uvs[1];

            /** COLOR */
            vertices[b + c++] = vertex.color[0];
            vertices[b + c++] = vertex.color[1];
            vertices[b + c++] = vertex.color[2];
            vertices[b + c++] = vertex.color[3];

            /** NORMAL */
            vertices[b + c++] = vertex.normal[0];
            vertices[b + c++] = vertex.normal[1];
            vertices[b + c++] = vertex.normal[2];
        }

        u32 text_indices[6] = {0, 1, 2, 2, 3, 0};
        for (int in = 0; in < DEFAULT_QUAD_INDICES_LEN; in++)
        {
            int index = (i * DEFAULT_QUAD_INDICES_LEN) + in;
            indices[index] = text_indices[in] + (4 * i);
        }

        free(uvs);
        free(quad_vertices);
    }

    render_item->vertices = vertices;
    render_item->indices = indices;

    render_item->size[0] = size[0];
    render_item->size[1] = size[1];

    render_item->should_ignore_camera = 1;
    add_shader(render_item, 0);
    bind_render_item_data(render_item);
    add_texture(render_item, "assets/UI/Text.png");

    return add_render_item(render_item);
};

void update_text_item(Render_Item *render_item, char *c, vec3 pos)
{
    int updated_length = 0;
    int length = strlen(c);

    int new_vertices_len = length * DEFAULT_QUAD_VERTICES_LEN;
    int new_indices_len = length * DEFAULT_QUAD_INDICES_LEN;

    if (render_item->vertices_len != new_vertices_len)
    {
        updated_length = 1;
        render_item->vertices_len = new_vertices_len;
        render_item->indices_len = new_indices_len;

        render_item->vertices = realloc(render_item->vertices, render_item->vertices_len * sizeof(f32));
        render_item->indices = realloc(render_item->indices, render_item->indices_len * sizeof(u32));
    }

    float overlap_ratio = 0.8 * render_item->size[0];
    for (int i = 0; i < length; i++)
    {
        int char_int = c[i];
        int x = ((char_int - 33) % 11);
        int y = floor((char_int - 33) / 11);

        int offset[2] = {x + 1, y + 1};
        int sprite_sheet_count[2] = {11, 9};
        int sprite_size[2] = {1, 1};

        float *uvs = get_uvs(offset, sprite_sheet_count, sprite_size);
        Vertex *quad_vertices = create_quad_vertices(pos[0] + (i * overlap_ratio), pos[1], 0, render_item->size, uvs);

        for (int j = 0; j < 4; j++)
        {
            Vertex vertex = quad_vertices[j];
            int b = (i * DEFAULT_QUAD_VERTICES_LEN) + j * sizeof(vertex) / 4;
            int c = 0;

            /** POS */
            render_item->vertices[b + c++] = vertex.pos[0];
            render_item->vertices[b + c++] = vertex.pos[1];
            render_item->vertices[b + c++] = vertex.pos[2];

            /** UVS */
            render_item->vertices[b + c++] = vertex.uvs[0];
            render_item->vertices[b + c++] = vertex.uvs[1];

            /** COLOR */
            render_item->vertices[b + c++] = vertex.color[0];
            render_item->vertices[b + c++] = vertex.color[1];
            render_item->vertices[b + c++] = vertex.color[2];
            render_item->vertices[b + c++] = vertex.color[3];

            /** NORMAL */
            render_item->vertices[b + c++] = vertex.normal[0];
            render_item->vertices[b + c++] = vertex.normal[1];
            render_item->vertices[b + c++] = vertex.normal[2];
        }

        u32 text_indices[6] = {0, 1, 2, 2, 3, 0};
        for (int in = 0; in < DEFAULT_QUAD_INDICES_LEN; in++)
        {
            int index = (i * DEFAULT_QUAD_INDICES_LEN) + in;
            render_item->indices[index] = text_indices[in] + (4 * i);
        }

        free(quad_vertices);
        free(uvs);
    }

    if (updated_length)
    {
        bind_vao(render_item->vao);
        bind_vbo(render_item->vbo);

        /** update buffer sizes */
        glBufferData(GL_ARRAY_BUFFER, render_item->vertices_len * sizeof(f32), render_item->vertices, GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, render_item->indices_len * sizeof(u32), render_item->indices, GL_STATIC_DRAW);

        unbind_vao(render_item->vao);
        unbind_vbo(render_item->vbo);
    }

    render_item->updated = 1;
};
