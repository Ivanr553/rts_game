#include "../../../../engine/engine.h"

#include "command_board.h"
#include "../../game_entities_internal.h"
#include "../../components/components.h"
#include "../../../state/state.h"

void init_command_board(void)
{
    float width = 1.5;
    float height = width * (float)((double)viewportWidth / (double)viewportHeight) * (float)((double)120 / (double)500);
    int sprite_sheet_size[2] = {1, 1};
    int sprite_size[2] = {500, 120};
    vec2 size = {width, height};
    vec3 pos = {1 - width / 2, -height * 1.065, 0};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_COMMAND_BOARD;
    entity->offset[0] = 1;
    entity->offset[1] = 1;
    entity->size[0] = size[0];
    entity->size[1] = size[1];

    /** Base Class*/
    Game_Entity *command_board = create_game_entity(entity, game_global.game_stores.in_game_store.player->player_slot);
    add_ui_component(command_board, _UI_TYPE, NULL, NULL, NULL);

    // /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Max-Controls.png");
    add_sprite_sheet_data(render_item, sprite_size, sprite_sheet_size);
    append_item_to_render_item(render_item, entity);
    render_item->should_ignore_camera = 1;

    // /** Render Item */
    init_render_item(render_item, pos, entity->size, NULL, NULL, entity->offset, NULL);
    bind_render_item_data(render_item);
};