#include "command_board.h"

#include "../../../../engine/engine.h"
#include "../../game_entities_internal.h"
#include "../../../state/state.h"
#include "../../../../generic/generic.h"

int *get_offset(UNIT_TYPE unit_type)
{
    int *offset = calloc(2, sizeof(int));
    switch (unit_type)
    {
    case UNIT_TYPE_MAX:
        offset[0] = 1;
        offset[1] = 5;
        break;

    case UNIT_TYPE_ALF:
        offset[0] = 1;
        offset[1] = 10;
        break;
    }

    return offset;
}

void select_unit(Game_Entity *game_entity)
{
    Game_Entity *unit_pointer = game_entity->ui_component->game_entity_pointer;

    Array *selected_unit_ids = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));
    append_array(selected_unit_ids, &unit_pointer->entity->id);

    select_units(selected_unit_ids);
}

Game_Entity *create_selection_icon(Game_Entity *selected_game_entity, int index)
{
    /** Initializing data */
    float width = 0.059;
    float height = width * (float)((double)viewportWidth / (double)viewportHeight);

    int *offset = get_offset(selected_game_entity->unit_type);
    int sprite_sheet_size[2] = {TILE_SPRITE_SHEET_COUNT, TILE_SPRITE_SHEET_COUNT};
    int sprite_size[2] = {16, 16};
    vec2 size = {width, height};

    float *pos = calloc(3, sizeof(float));
    pos[0] = -0.325 + ((index % (SELECTED_UNIT_MAX_COUNT / 2)) * 0.08);
    pos[1] = -0.725 - (floor(index / (SELECTED_UNIT_MAX_COUNT / 2)) * 0.13);
    pos[2] = 0;

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UI;

    /** Base Class*/
    Game_Entity *game_entity = create_game_entity(entity);
    void *on_click = select_unit;
    add_ui_component(game_entity, on_click);
    game_entity->ui_component->game_entity_pointer = selected_game_entity;

    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Command-Buttons.png");
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    /** Render Item */
    render_item->should_ignore_camera = 1;

    add_sprite_sheet_data(render_item, offset, sprite_size, sprite_sheet_size);
    init_render_item(render_item, pos, size, NULL, NULL);
    bind_render_item_data(render_item);

    return game_entity;
}

void generate_selection_icons(void)
{
    Array *old_selected_unit_icon_ids = game_global.game_stores.in_game_store.selected_unit_icon_ids;
    printf("len: %d\n", old_selected_unit_icon_ids->len);
    for (int i = 0; i < old_selected_unit_icon_ids->len; i++)
    {
        long entity_id = *((long *)get_item_from_array(old_selected_unit_icon_ids, i));
        Entity *entity = get_entity_by_id(entity_id);

        remove_entity(entity);
    }

    Array *selected_unit_ids = game_global.game_stores.in_game_store.selected_units_by_id;
    Array *selected_unit_icon_ids = create_array(SELECTED_UNIT_MAX_COUNT, sizeof(long));

    for (int i = 0; i < selected_unit_ids->len; i++)
    {
        long entity_id = *((long *)get_item_from_array(selected_unit_ids, i));
        Entity *entity = get_entity_by_id(entity_id);
        Game_Entity *game_entity = entity->entity_class;

        Game_Entity *selection_icon_game_entity = create_selection_icon(game_entity, i);
        add_entity(selection_icon_game_entity->entity);
        append_array(selected_unit_icon_ids, &selection_icon_game_entity->entity->id);
    }

    game_global.game_stores.in_game_store.selected_unit_icon_ids = selected_unit_icon_ids;
};
