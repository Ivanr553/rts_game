#include "../../../../engine/engine.h"

#include "command_board.h"
#include "../../game_entities_internal.h"
#include "../../../state/game_global.h"
#include "../building_selection/building_selection.h"
#include "../../components/components.h"

void command_button_build(void)
{
    if (game_global.game_stores.in_game_store.is_placing_building)
    {
        game_global.game_stores.in_game_store.is_placing_building = 0;
        game_global.game_stores.in_game_store.building_being_placed = 0;
        hide_building_selection();
    }
    else
    {
        game_global.game_stores.in_game_store.is_placing_building = 1;
        game_global.game_stores.in_game_store.building_being_placed = BUILDING_SUMMONING_CIRCLE;
        show_building_selection(6, 5);
    }
}

void *get_on_click(COMMAND_BUTTON button)
{
    switch (button)
    {
    case COMMAND_BUTTON_BUILD:
        return command_button_build;
        break;

    default:
        return NULL;
    }
}

void init_command_board_buttons(int x, int y, int offset[2], COMMAND_BUTTON button)
{
    /** Initializing data */
    float width = 0.059;
    float height = width * (float)((double)viewportWidth / (double)viewportHeight);

    int sprite_sheet_size[2] = {TILE_SPRITE_SHEET_COUNT, TILE_SPRITE_SHEET_COUNT};
    int sprite_size[2] = {16, 16};
    vec2 size = {width, height};

    float *pos = calloc(3, sizeof(float));
    pos[0] = 0.6515 + (x * 0.0665);
    pos[1] = -0.555 - (y * 0.11655);
    pos[2] = 0;

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UI;

    /** Base Class*/
    Game_Entity *game_entity = create_game_entity(entity);
    void *on_click = get_on_click(button);
    add_ui_component(game_entity, on_click);

    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Command-Buttons.png");
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