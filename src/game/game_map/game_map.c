#include <json/json.h>
#include <linmath.h>

#include "game_map.h"

#include "../../generic/generic.h"

#include "../../engine/engine.h"

#include "../state/game_global.h"
#include "../game_entities/game_entities.h"

void init_game_map(Game_Map *game_map, char *file_path)
{
    File json_file = io_file_read(file_path);

    json_value *json_data = json_parse(json_file.data, json_file.len);

    json_value *layers = json_data->u.object.values[3].value;
    json_value *layer = layers->u.array.values[0];
    json_value *data = layer->u.object.values[0].value;
    int data_length = data->u.array.length;

    int *tile_set = calloc(data_length, sizeof(int));
    for (int i = 0; i < data_length; i++)
    {
        tile_set[i] = data->u.array.values[i]->u.integer;
    }

    game_map->tile_set = tile_set;
    game_map->tile_set_len = data_length;
    game_map->x_len = (int)layer->u.object.values[7].value->u.integer;
    game_map->y_len = (int)layer->u.object.values[1].value->u.integer;
    game_map->width = game_map->x_len * TILE_SPRITE_SIZE;
    game_map->height = game_map->y_len * TILE_SPRITE_SIZE;

    game_map->start_x = 23 - (game_map->x_len / 2);
    game_map->start_y = (game_map->y_len / 2) - 19;

    game_global.game_stores.in_game_store.map = game_map;

    create_map_from_tiles();
};

void create_map_from_tiles(void)
{
    f32 *vertices;
    u32 *indices;

    Render_Item *render_item = get_render_item(0, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/maps/Tile_Set.png");
    // game_global.game_stores.in_game_store.map->tile_set_len = 10;

    render_item->vertices_len = game_global.game_stores.in_game_store.map->tile_set_len * DEFAULT_QUAD_VERTICES_LEN;
    vertices = calloc(render_item->vertices_len, sizeof(f32));
    render_item->indices_len = game_global.game_stores.in_game_store.map->tile_set_len * DEFAULT_QUAD_INDICES_LEN;
    indices = calloc(render_item->indices_len, sizeof(u32));
    render_item->vertex_stride = DEFAULT_QUAD_STRIDE;
    render_item->index_stride = DEFAULT_QUAD_INDICES_LEN;

    Array *resource_positions = create_array(100, sizeof(float) * 2);

    for (int i = 0; i < game_global.game_stores.in_game_store.map->tile_set_len; i++)
    {
        int tile = game_global.game_stores.in_game_store.map->tile_set[i];

        float col = i % game_global.game_stores.in_game_store.map->x_len;
        float row = floor(i / game_global.game_stores.in_game_store.map->x_len);

        float x = -((game_global.game_stores.in_game_store.map->x_len) / 2) + (col);
        float y = ((game_global.game_stores.in_game_store.map->y_len) / 2) + (-row);

        if (tile == 91)
        {
            float *resource_position = calloc(2, sizeof(float));
            resource_position[0] = x;
            resource_position[1] = y;
            append_array(resource_positions, resource_position);
        }

        int offset[2] = {tile % TILE_SPRITE_SHEET_COUNT, (int)(tile / TILE_SPRITE_SHEET_COUNT) + 1};
        int sprite_sheet_count[2] = {TILE_SPRITE_SHEET_COUNT, TILE_SPRITE_SHEET_COUNT};
        int sprite_size[2] = {1, 1};
        vec2 size = {MAP_TILE_SIZE, MAP_TILE_SIZE};

        float *uvs = get_uvs(offset, sprite_sheet_count, sprite_size);
        Vertex *quad_vertices = create_quad_vertices(x, y, 0, size, uvs);

        /** TODO: Determine if we really need tile entities */

        // Entity *tile_entity = calloc(1, sizeof(Entity));
        // tile_entity->id = entity_id_counter++;
        // Render_Item *tile_render_item = create_render_item(RENDER_ITEM_QUAD);
        // tile_render_item->is_static_object = 1;

        // f32 *positional_vertices = calloc(12, sizeof(f32));

        for (int j = 0; j < 4; j++)
        {
            Vertex vertex = quad_vertices[j];
            int b = (i * DEFAULT_QUAD_VERTICES_LEN) + j * sizeof(vertex) / 4;
            int c = 0;

            /** POS */
            vertices[b + c++] = vertex.pos[0];
            vertices[b + c++] = vertex.pos[1];
            vertices[b + c++] = vertex.pos[2];

            // positional_vertices[(j * 3) + c - 3] = vertex.pos[0] + (col);
            // positional_vertices[(j * 3) + c - 2] = vertex.pos[1] + (-row);
            // positional_vertices[(j * 3) + c - 1] = vertex.pos[2];

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

        // tile_render_item->positional_vertices = positional_vertices;
        // tile_render_item->positional_vertices_len = 12;
        // tile_entity->render_item = tile_render_item;
        // append_array(global.render.entities, &tile_entity);

        u32 map_tile_indices[6] = {0, 1, 2, 2, 3, 0};
        for (int in = 0; in < DEFAULT_QUAD_INDICES_LEN; in++)
        {
            int index = (i * DEFAULT_QUAD_INDICES_LEN) + in;
            indices[index] = map_tile_indices[in] + (4 * i);
        }

        free(uvs);
        free(quad_vertices);
    }

    render_item->vertices = vertices;
    render_item->indices = indices;

    render_item->opacity = 1;

    vec3 pos = {0, 0, 0};

    bind_render_item_data(render_item);

    Entity *map_entity = create_entity(NULL, pos);
    map_entity->render_item = render_item;

    generate_resources(resource_positions);
};

void snap_to_map_grid(float *pos, float *size)
{
    float mouse_pos[3];
    get_mouse_pos_on_map(mouse_pos);

    float size_offset_x = (int)size[0] % 2 ? 0 : 0.5;
    float size_offset_y = (int)size[1] % 2 ? 0 : 0.5;

    pos[0] = roundf(mouse_pos[0] - size_offset_x) + size_offset_x;
    pos[1] = floorf(mouse_pos[1] + 0.5 - size_offset_y) + size_offset_y;
};

void generate_resources(Array *resource_positions)
{
    for (int i = 0; i < resource_positions->len; i++)
    {
        float *pos = get_item_from_array(resource_positions, i);

        float *_pos = calloc(3, sizeof(float));
        _pos[0] = pos[0];
        _pos[1] = pos[1];
        _pos[2] = DEFAULT_UNIT_Z;

        create_resource(RESOURCE_CRYSTAL, _pos);
    }

    free(resource_positions);
};