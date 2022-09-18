#include "player_ui.h"

#include "../../../../engine/engine.h"
#include "../../game_entities.h"
#include "../../../state/state.h"
#include "../../components/components.h"

void init_player_crystal_icon(void)
{
    /** Initializing data */
    float width = 0.04;
    float height = width * (float)((double)viewportWidth / (double)viewportHeight);

    int sprite_sheet_size[2] = {TILE_SPRITE_SHEET_COUNT, TILE_SPRITE_SHEET_COUNT};
    int offset[2] = {1, 1};
    int sprite_size[2] = {1, 1};
    vec2 size = {width, height};

    float *pos = calloc(3, sizeof(float));
    pos[0] = 0.825;
    pos[1] = 0.95;
    pos[2] = 0;

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UI;

    /** Base Class*/
    Game_Entity *entity_class = create_game_entity(entity);
    add_ui_component(entity_class, NULL);

    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Icons.png");
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        /** Render Item */
        render_item->should_ignore_camera = 1;

        add_sprite_sheet_data(render_item, offset, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, size, NULL, NULL);
        bind_render_item_data(render_item);
    }

    add_entity(entity);
}

void update_player_crystal_text(Entity *entity)
{
    int crystal_count = game_global.game_stores.in_game_store.player->crystal_count;
    char frame_data[3];
    sprintf(frame_data, "%d", crystal_count);
    update_text_item(entity->render_item, frame_data, entity->pos);
}

void init_player_crystal_text(void)
{
    /** Initializing data */
    float width = 0.04;
    float height = width * (float)((double)viewportWidth / (double)viewportHeight);
    int sprite_sheet_size[2] = {TILE_SPRITE_SHEET_COUNT, TILE_SPRITE_SHEET_COUNT};
    int offset[2] = {1, 1};
    int sprite_size[2] = {1, 1};
    vec2 size = {width, height};

    float *pos = calloc(3, sizeof(float));
    pos[0] = 0.87;
    pos[1] = 0.95;
    pos[2] = 0;

    Entity *crystal_text_entity = create_entity(NULL, pos);
    crystal_text_entity->render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Text.png");
    crystal_text_entity->render_item->should_ignore_camera = 1;
    crystal_text_entity->update_entity = update_player_crystal_text;

    add_sprite_sheet_data(crystal_text_entity->render_item, offset, sprite_size, sprite_sheet_size);
    init_render_item(crystal_text_entity->render_item, pos, size, NULL, NULL);
    bind_render_item_data(crystal_text_entity->render_item);
    add_entity(crystal_text_entity);
}

void init_player_resources(void)
{
    init_player_crystal_icon();
    init_player_crystal_text();
}
