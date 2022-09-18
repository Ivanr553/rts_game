#include "../../../../engine/engine.h"

#include "command_board.h"
#include "../../game_entities_internal.h"
#include "../../components/components.h"

void init_command_board(void)
{
    float width = 1.5;
    float height = width * (float)((double)viewportWidth / (double)viewportHeight) * (float)((double)120 / (double)500);
    int offset[2] = {1, 1};
    int sprite_sheet_size[2] = {1, 1};
    int sprite_size[2] = {500, 120};
    vec2 size = {width, height};
    vec3 pos = {1 - width / 2, -height * 1.065, 0};

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_COMMAND_BOARD;

    /** Base Class*/
    Game_Entity *command_board = create_game_entity(entity);
    add_ui_component(entity, NULL);

    // /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Max-Controls.png");
    add_sprite_sheet_data(render_item, offset, sprite_size, sprite_sheet_size);
    append_item_to_render_item(render_item, entity);
    render_item->should_ignore_camera = 1;

    // /** Render Item */
    init_render_item(render_item, pos, size, NULL, NULL);
    bind_render_item_data(render_item);

    for (int i = 0; i < 4; i++)
    {
        int offset[2] = {i + 1, 1};
        init_command_board_buttons(i, 0, offset, COMMAND_BUTTON_MOVE);
    }

    int offset_2[2] = {5, 1};
    init_command_board_buttons(0, 3, offset_2, COMMAND_BUTTON_BUILD);

    return command_board;
};