#include "command_board.h"

#include "../../../../engine/engine.h"
#include "../../../state/state.h"

float building_queue_box_width = 0.055;
float progress_bar_max_width = (0.055 * 3) + (0.015 * 2);
float progress_bar_starting_pos = -0.056;

void update_command_board_info_entity(Game_Entity *game_entity)
{
    Game_Entity *parent_entity = game_entity->ui_component->game_entity_pointer;
    if (parent_entity->building_component->queued_builds->len > 0)
    {
        switch (game_entity->ui_component->type)
        {
        case UI_TYPE_COMMAND_BOARD_QUEUE_1:
            if (parent_entity->building_component->queued_builds->len >= 1)
            {
                Queued_Build *queued_build = get_item_from_array(parent_entity->building_component->queued_builds, 0);
                game_entity->entity->offset[0] = queued_build->icon_offset[0];
                game_entity->entity->offset[1] = queued_build->icon_offset[1];
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            else if (game_entity->entity->offset[0] != 0 && game_entity->entity->offset[1] != 6)
            {
                game_entity->entity->offset[0] = 1;
                game_entity->entity->offset[1] = 6;
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            break;
        case UI_TYPE_COMMAND_BOARD_QUEUE_2:
            if (parent_entity->building_component->queued_builds->len >= 2)
            {
                Queued_Build *queued_build = get_item_from_array(parent_entity->building_component->queued_builds, 1);
                game_entity->entity->offset[0] = queued_build->icon_offset[0];
                game_entity->entity->offset[1] = queued_build->icon_offset[1];
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            else if (game_entity->entity->offset[0] != 0 && game_entity->entity->offset[1] != 6)
            {
                game_entity->entity->offset[0] = 1;
                game_entity->entity->offset[1] = 6;
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            break;
        case UI_TYPE_COMMAND_BOARD_QUEUE_3:
            if (parent_entity->building_component->queued_builds->len >= 3)
            {
                Queued_Build *queued_build = get_item_from_array(parent_entity->building_component->queued_builds, 2);
                game_entity->entity->offset[0] = queued_build->icon_offset[0];
                game_entity->entity->offset[1] = queued_build->icon_offset[1];
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            else if (game_entity->entity->offset[0] != 0 && game_entity->entity->offset[1] != 6)
            {
                game_entity->entity->offset[0] = 1;
                game_entity->entity->offset[1] = 6;
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            break;
        case UI_TYPE_COMMAND_BOARD_QUEUE_4:
            if (parent_entity->building_component->queued_builds->len >= 4)
            {
                Queued_Build *queued_build = get_item_from_array(parent_entity->building_component->queued_builds, 3);
                game_entity->entity->offset[0] = queued_build->icon_offset[0];
                game_entity->entity->offset[1] = queued_build->icon_offset[1];
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            else if (game_entity->entity->offset[0] != 0 && game_entity->entity->offset[1] != 6)
            {
                game_entity->entity->offset[0] = 1;
                game_entity->entity->offset[1] = 6;
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            break;
        case UI_TYPE_COMMAND_BOARD_QUEUE_5:
            if (parent_entity->building_component->queued_builds->len >= 5)
            {
                Queued_Build *queued_build = get_item_from_array(parent_entity->building_component->queued_builds, 4);
                game_entity->entity->offset[0] = queued_build->icon_offset[0];
                game_entity->entity->offset[1] = queued_build->icon_offset[1];
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            else if (game_entity->entity->offset[0] != 0 && game_entity->entity->offset[1] != 6)
            {
                game_entity->entity->offset[0] = 1;
                game_entity->entity->offset[1] = 6;
                game_entity->entity->render_item->updated = 1;
                game_entity->entity->render_item->should_update = 1;
            }
            break;
        }
    }
    else if (game_entity->entity->offset[0] != 0 && game_entity->entity->offset[1] != 6)
    {
        game_entity->entity->offset[0] = 1;
        game_entity->entity->offset[1] = 6;
        game_entity->entity->render_item->updated = 1;
        game_entity->entity->render_item->should_update = 1;
    }
}

void update_command_board_loading_bar(Game_Entity *game_entity)
{
    Game_Entity *parent_entity = game_entity->ui_component->game_entity_pointer;
    if (parent_entity->building_component->queued_builds->len > 0)
    {
        Queued_Build *queued_build = get_item_from_array(parent_entity->building_component->queued_builds, 0);

        game_entity->entity->size[0] = progress_bar_max_width * (float)(((double)queued_build->total_build_time - (double)queued_build->build_time) / (double)queued_build->total_build_time);
        game_entity->entity->pos[0] = progress_bar_starting_pos + (game_entity->entity->size[0] / 2);
        game_entity->entity->render_item->should_update = 1;
        game_entity->entity->render_item->updated = 1;
    }
    else if (game_entity->entity->size[0] > 0)
    {
        game_entity->entity->size[0] = 0;
        game_entity->entity->pos[0] = progress_bar_starting_pos;
        game_entity->entity->render_item->should_update = 1;
        game_entity->entity->render_item->updated = 1;
    }
}

Game_Entity *get_command_board_info_entity(float *pos, int *offset, Game_Entity *parent_entity, int index)
{
    /** Initializing data */
    float height = building_queue_box_width * (float)((double)viewportWidth / (double)viewportHeight);

    int sprite_sheet_size[2] = {TILE_SPRITE_SHEET_COUNT, TILE_SPRITE_SHEET_COUNT};
    int sprite_size[2] = {1, 1};
    vec2 size = {building_queue_box_width, height};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UI;
    entity->offset[0] = offset[0];
    entity->offset[1] = offset[1];
    entity->size[0] = size[0];
    entity->size[1] = size[1];

    /** Base Class*/
    Game_Entity *game_entity = create_game_entity(entity, game_global.game_stores.in_game_store.player->player_slot);
    switch (index)
    {
    case 0:
        add_ui_component(game_entity, UI_TYPE_COMMAND_BOARD_QUEUE_1, parent_entity, NULL, update_command_board_info_entity);
        break;
    case 1:
        add_ui_component(game_entity, UI_TYPE_COMMAND_BOARD_QUEUE_2, parent_entity, NULL, update_command_board_info_entity);
        break;
    case 2:
        add_ui_component(game_entity, UI_TYPE_COMMAND_BOARD_QUEUE_3, parent_entity, NULL, update_command_board_info_entity);
        break;
    case 3:
        add_ui_component(game_entity, UI_TYPE_COMMAND_BOARD_QUEUE_4, parent_entity, NULL, update_command_board_info_entity);
        break;
    case 4:
        add_ui_component(game_entity, UI_TYPE_COMMAND_BOARD_QUEUE_5, parent_entity, NULL, update_command_board_info_entity);
        break;
    }
    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Command-Buttons.png");
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        /** Render Item */
        render_item->should_ignore_camera = 1;

        add_sprite_sheet_data(render_item, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, entity->size, NULL, NULL, entity->offset, NULL, 1);
        bind_render_item_data(render_item);
    }

    return game_entity;
};

Game_Entity *get_loading_bar_entity(Game_Entity *game_entity)
{
    int *offset = calloc(2, sizeof(int));
    offset[0] = 5;
    offset[1] = 2;
    /** Initializing data */
    float width = 0;
    float height = 0.01 * (float)((double)viewportWidth / (double)viewportHeight);
    float *pos = calloc(3, sizeof(float));
    pos[0] = progress_bar_starting_pos;
    pos[1] = -0.7351 - (height * 2);
    pos[2] = 0;

    int sprite_sheet_size[2] = {TILE_SPRITE_SHEET_COUNT, TILE_SPRITE_SHEET_COUNT};
    int sprite_size[2] = {1, 1};
    vec2 size = {width, height};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UI;
    entity->offset[0] = 5;
    entity->offset[1] = 2;
    entity->size[0] = size[0];
    entity->size[1] = size[1];

    /** Base Class*/
    Game_Entity *loading_entity = create_game_entity(entity, game_global.game_stores.in_game_store.player->player_slot);
    add_ui_component(loading_entity, _UI_TYPE, game_entity, NULL, update_command_board_loading_bar);

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Icons.png");
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        /** Render Item */
        render_item->should_ignore_camera = 1;

        add_sprite_sheet_data(render_item, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, entity->size, NULL, NULL, entity->offset, NULL, 1);
        bind_render_item_data(render_item);
    }

    return loading_entity;
};

void show_unit_command_board_info(Game_Entity *game_entity){};

void show_building_command_board_info(Game_Entity *game_entity)
{
    Array *new_command_board_info_ids = create_array(COMMAND_BOARD_INFO_COUNT, sizeof(long));

    float command_board_info_data[20] = {
        -0.1,
        -0.7351,
        1,
        6,
        -0.1,
        -0.86,
        1,
        6,
        -0.03,
        -0.86,
        1,
        6,
        0.04,
        -0.86,
        1,
        6,
        0.11,
        -0.86,
        1,
        6,
    };

    for (int i = 0; i < 20; i += 4)
    {
        float *pos = calloc(3, sizeof(float));
        pos[0] = command_board_info_data[i];
        pos[1] = command_board_info_data[i + 1];
        pos[2] = 0;
        int *offset = calloc(2, sizeof(int));
        offset[0] = (int)command_board_info_data[i + 2];
        offset[1] = (int)command_board_info_data[i + 3];
        Game_Entity *command_board_info_entity = get_command_board_info_entity(pos, offset, game_entity, floor(i / 4));

        append_array(new_command_board_info_ids, &command_board_info_entity->entity->id);
        add_entity(command_board_info_entity->entity);
    }

    Game_Entity *loading_entity = get_loading_bar_entity(game_entity);
    append_array(new_command_board_info_ids, &loading_entity->entity->id);
    add_entity(loading_entity->entity);

    game_global.game_stores.in_game_store.command_board_info_ids = new_command_board_info_ids;
};

void show_command_board_info(Game_Entity *game_entity)
{
    if (game_entity->building_component)
    {
        show_building_command_board_info(game_entity);
        return;
    }

    show_unit_command_board_info(game_entity);
}