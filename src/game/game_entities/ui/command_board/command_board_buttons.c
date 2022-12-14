#include "../../../../engine/engine.h"

#include "command_board.h"
#include "../../game_entities_internal.h"
#include "../../../state/game_global.h"
#include "../building/building_ui.h"
#include "../../components/components.h"
#include "../../../state/events/events.h"

void command_button_stop(void)
{
    Array *selected_unit_ids = game_global.game_stores.in_game_store.selected_units_by_id;

    for (int i = 0; i < selected_unit_ids->len; i++)
    {
        long entity_id = *((long *)get_item_from_array(selected_unit_ids, i));
        Entity *entity = get_entity_by_id(entity_id);
        Game_Entity *game_entity = entity->entity_class;

        if (game_entity->entity->should_move_to_pos)
        {
            game_entity->entity->should_move_to_pos = 0;
        }

        if (game_entity->harvester_component)
        {
            stop_harvesting(game_entity);
        }
    }
};

void command_button_build(void)
{
    int *buttons = get_building_command_buttons();
    generate_command_buttons(buttons, NULL);
};

void create_max(Game_Entity *building_game_component)
{
    Array *_array = create_array(1, sizeof(Game_Entity));
    Game_Entity *worker = create_worker((vec3){building_game_component->entity->pos[0] + building_game_component->entity->unit_radius + (float)(rand() % 20) / 100, building_game_component->entity->pos[1] + building_game_component->entity->unit_radius + (float)(rand() % 20) / 100, DEFAULT_UNIT_Z}, GAME_ENTITY_TYPE_MAX);

    append_array(_array, &worker->entity->id);
    if (!handle_right_click_unit_interaction(building_game_component->building_component->rally_point, _array))
    {
        move_to(worker->entity, building_game_component->building_component->rally_point);
    }

    free(_array);
}

void create_spike(Game_Entity *building_game_component)
{
    Game_Entity *spike = create_unit((vec3){building_game_component->entity->pos[0] + building_game_component->entity->unit_radius + (float)(rand() % 20) / 100, building_game_component->entity->pos[1] + building_game_component->entity->unit_radius + (float)(rand() % 20) / 100, DEFAULT_UNIT_Z}, GAME_ENTITY_TYPE_SPIKE);
    move_to(spike->entity, building_game_component->building_component->rally_point);
}

void command_button_create_unit(Game_Entity *ui_game_entity)
{
    if (!spend_resources(50, 0))
    {
        printf("Not enough resources");
        return;
    }

    Game_Entity *building_game_entity = ui_game_entity->ui_component->game_entity_pointer;
    Component_Building *building_component = building_game_entity->building_component;

    Queued_Build *queued_build = calloc(1, sizeof(Queued_Build));
    queued_build->build_time = 1300;
    queued_build->total_build_time = 1300;
    queued_build->icon_offset[0] = 1;
    queued_build->icon_offset[1] = 3;
    queued_build->create_unit = create_max;
    queued_build->rally_point[0] = building_component->rally_point[0];
    queued_build->rally_point[1] = building_component->rally_point[1];
    queued_build->rally_point[2] = building_component->rally_point[2];

    append_array(building_component->queued_builds, queued_build);
};

void command_button_build_spike(Game_Entity *ui_game_entity)
{
    if (!spend_resources(100, 0))
    {
        printf("Not enough resources");
        return;
    }

    Game_Entity *building_game_entity = ui_game_entity->ui_component->game_entity_pointer;
    Component_Building *building_component = building_game_entity->building_component;

    Queued_Build *queued_build = calloc(1, sizeof(Queued_Build));
    queued_build->build_time = 800;
    queued_build->total_build_time = 800;
    queued_build->icon_offset[0] = 1;
    queued_build->icon_offset[1] = 3;
    queued_build->create_unit = create_spike;
    queued_build->rally_point[0] = building_component->rally_point[0];
    queued_build->rally_point[1] = building_component->rally_point[1];
    queued_build->rally_point[2] = building_component->rally_point[2];

    append_array(building_component->queued_builds, queued_build);
};

void command_button_build_base(Game_Entity *ui_game_entity)
{

    if (!spend_resources(400, 0))
    {
        printf("Not enough resources");
        return;
    }

    if (ui_game_entity->ui_component->game_entity_pointer)
    {
        printf("No parent entity pointer when attempting to build base\n");
        return;
    }

    if (game_global.game_stores.in_game_store.is_placing_building)
    {
        hide_building_selection();
    }
    else
    {
        game_global.game_stores.in_game_store.building_being_placed = BUILDING_TYPE_BASE;
        show_building_selection(3, 3);
    }
}

void command_button_build_ram(Game_Entity *ui_game_entity)
{

    if (!can_spend_resources(100, 0))
    {
        printf("Not enough resources");
        return;
    }

    if (ui_game_entity->ui_component->game_entity_pointer)
    {
        printf("No parent entity pointer when attempting to factory\n");
        return;
    }

    if (game_global.game_stores.in_game_store.is_placing_building)
    {
        hide_building_selection();
    }
    else
    {
        game_global.game_stores.in_game_store.building_being_placed = BUILDING_TYPE_RAM;
        show_building_selection(3, 3);
    }
}

void command_button_build_factory(Game_Entity *ui_game_entity)
{

    if (!can_spend_resources(150, 0))
    {
        printf("Not enough resources");
        return;
    }

    if (ui_game_entity->ui_component->game_entity_pointer)
    {
        printf("No parent entity pointer when attempting to factory\n");
        return;
    }

    if (game_global.game_stores.in_game_store.is_placing_building)
    {
        hide_building_selection();
    }
    else
    {
        game_global.game_stores.in_game_store.building_being_placed = BUILDING_TYPE_FACTORY;
        show_building_selection(3, 3);
    }
}

void *get_on_click_and_bind_entity(Game_Entity *game_entity, COMMAND_BUTTON button)
{
    switch (button)
    {
    case COMMAND_BUTTON_BUILD:
        return command_button_build;
    case COMMAND_BUTTON_ESCAPE:
        return show_command_board_buttons_by_selected_unit;
    case COMMAND_BUTTON_STOP:
        return command_button_stop;
    case COMMAND_BUTTON_BUILD_MAX:
        return command_button_create_unit;
    case COMMAND_BUTTON_BUILD_SPIKE:
        return command_button_build_spike;
    case COMMAND_BUTTON_BUILD_BASE:
        return command_button_build_base;
    case COMMAND_BUTTON_BUILD_FACTORY:
        return command_button_build_factory;
    case COMMAND_BUTTON_BUILD_RAM:
        return command_button_build_ram;

    default:
        return NULL;
    }
}

int *get_button_offset(COMMAND_BUTTON button)
{
    int *offset = calloc(2, sizeof(int));
    switch (button)
    {
    case COMMAND_BUTTON_MOVE:
        offset[0] = 1;
        offset[1] = 1;
        break;
    case COMMAND_BUTTON_ATTACK:
        offset[0] = 2;
        offset[1] = 1;
        break;
    case COMMAND_BUTTON_HOLD:
        offset[0] = 3;
        offset[1] = 1;
        break;
    case COMMAND_BUTTON_STOP:
        offset[0] = 4;
        offset[1] = 1;
        break;
    case COMMAND_BUTTON_BUILD:
        offset[0] = 5;
        offset[1] = 1;
        break;
    case COMMAND_BUTTON_BUILD_BASE:
        offset[0] = 2;
        offset[1] = 3;
        break;
    case COMMAND_BUTTON_BUILD_FACTORY:
        offset[0] = 3;
        offset[1] = 3;
        break;
    case COMMAND_BUTTON_BUILD_RAM:
        offset[0] = 4;
        offset[1] = 3;
        break;
    case COMMAND_BUTTON_ESCAPE:
        offset[0] = 6;
        offset[1] = 1;
        break;
    case COMMAND_BUTTON_BUILD_SPIKE:
    case COMMAND_BUTTON_BUILD_MAX:
        offset[0] = 1;
        offset[1] = 3;
        break;

    default:
        offset[0] = 1;
        offset[1] = 1;
        break;
    }
    return offset;
};

Game_Entity *add_command_board_button(int index, COMMAND_BUTTON button, Game_Entity *parent_entity)
{
    /** Initializing data */
    float aspect_ratio = (float)((double)viewportWidth / (double)viewportHeight);
    float width = COMMAND_BOARD_BUTTON_SIZE;
    float height = width * aspect_ratio;
    float command_board_height = COMMABD_BOARD_WIDTH * aspect_ratio * (float)((double)120 / (double)500);

    int *offset = get_button_offset(button);
    int sprite_sheet_size[2] = {TILE_SPRITE_SHEET_COUNT, TILE_SPRITE_SHEET_COUNT};
    int sprite_size[2] = {16, 16};
    vec2 size = {width, height};

    float *pos = calloc(3, sizeof(float));
    pos[0] = COMMAND_BOARD_BUTTON_START_X + ((index % COMMAND_BOARD_BUTTON_ROW_COUNT) * (width + COMMAND_BOARD_BUTTON_GAP));
    pos[1] = (-1 + (command_board_height / 2)) - (COMMAND_BOARD_BUTTON_START_Y * aspect_ratio) - (floor(index / COMMAND_BOARD_BUTTON_ROW_COUNT) * (height + (COMMAND_BOARD_BUTTON_GAP * aspect_ratio)));
    pos[2] = 0;

    /** Entity */
    Entity *entity = create_entity(NULL, pos);
    entity->entity_class_type = ENTITY_CLASS_UI;
    entity->offset[0] = offset[0];
    entity->offset[1] = offset[1];
    entity->size[0] = size[0];
    entity->size[1] = size[1];

    /** Base Class*/
    Game_Entity *game_entity = create_game_entity(entity, game_global.game_stores.in_game_store.player->player_slot);
    void *on_click = get_on_click_and_bind_entity(game_entity, button);
    add_ui_component(game_entity, UI_TYPE_COMMAND_BOARD_BUTTON_1 + index, parent_entity, on_click, NULL);

    /** Render Item */
    Render_Item *render_item = get_render_item(0, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Command-Buttons.png");
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
}

void generate_command_buttons(int *buttons, Game_Entity *parent_entity)
{

    Array *old_command_button_ids = game_global.game_stores.in_game_store.command_button_ids;
    for (int i = 0; i < old_command_button_ids->len; i++)
    {
        long entity_id = *((long *)get_item_from_array(old_command_button_ids, i));
        Entity *entity = get_entity_by_id(entity_id);

        remove_entity(entity);
    }

    Array *command_button_ids = create_array(COMMAND_BOARD_BUTTON_COUNT, sizeof(long));
    for (int i = 0; i < COMMAND_BOARD_BUTTON_COUNT; i++)
    {
        int command_button = buttons[i];
        if (command_button == 0)
        {
            continue;
        }
        Game_Entity *button_entity = add_command_board_button(i, command_button, parent_entity);
        add_entity(button_entity->entity);
        append_array(command_button_ids, &button_entity->entity->id);
    }

    game_global.game_stores.in_game_store.command_button_ids = command_button_ids;
};

void show_command_board_buttons_by_selected_unit(void)
{

    Array *selected_unit_ids = game_global.game_stores.in_game_store.selected_units_by_id;
    long entity_id = *((long *)get_item_from_array(selected_unit_ids, 0));
    Entity *entity = get_entity_by_id(entity_id);
    if (!entity)
    {
        return;
    }

    Game_Entity *game_entity = entity->entity_class;
    if (!game_entity || !game_entity->unit_type)
    {
        return;
    }

    int *buttons = get_command_buttons_by_unit_type(game_entity->unit_type);
    generate_command_buttons(buttons, game_entity);
}

int *get_command_buttons_by_unit_type(GAME_ENTITY_TYPE type)
{
    int *buttons = calloc(COMMAND_BOARD_BUTTON_COUNT, sizeof(int));
    switch (type)
    {
    case GAME_ENTITY_TYPE_MAX:
    case GAME_ENTITY_TYPE_ALF:
        int worker_buttons[COMMAND_BOARD_BUTTON_COUNT] = {
            COMMAND_BUTTON_MOVE, COMMAND_BUTTON_ATTACK, COMMAND_BUTTON_HOLD, COMMAND_BUTTON_STOP,
            0, 0, 0, 0,
            COMMAND_BUTTON_BUILD, 0, 0, 0};

        for (int i = 0; i < COMMAND_BOARD_BUTTON_COUNT; i++)
        {
            buttons[i] = worker_buttons[i];
        }
        break;

    case GAME_ENTITY_TYPE_SPIKE:
        int default_unit_buttons[COMMAND_BOARD_BUTTON_COUNT] = {
            COMMAND_BUTTON_MOVE, COMMAND_BUTTON_ATTACK, COMMAND_BUTTON_HOLD, COMMAND_BUTTON_STOP,
            0, 0, 0, 0,
            0, 0, 0, 0};

        for (int i = 0; i < COMMAND_BOARD_BUTTON_COUNT; i++)
        {
            buttons[i] = default_unit_buttons[i];
        }
        break;

    case GAME_ENTITY_TYPE_BASE:
        int base_buttons[COMMAND_BOARD_BUTTON_COUNT] = {
            COMMAND_BUTTON_BUILD_MAX, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0};

        for (int i = 0; i < COMMAND_BOARD_BUTTON_COUNT; i++)
        {
            buttons[i] = base_buttons[i];
        }
        break;

    case GAME_ENTITY_TYPE_FACTORY:
        int factory_buttons[COMMAND_BOARD_BUTTON_COUNT] = {
            COMMAND_BUTTON_BUILD_SPIKE, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0};

        for (int i = 0; i < COMMAND_BOARD_BUTTON_COUNT; i++)
        {
            buttons[i] = factory_buttons[i];
        }
        break;

    default:
        int default_buttons[COMMAND_BOARD_BUTTON_COUNT] = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0};

        for (int i = 0; i < COMMAND_BOARD_BUTTON_COUNT; i++)
        {
            buttons[i] = default_buttons[i];
        }
        break;
    }
    return buttons;
};

int *get_building_command_buttons(void)
{
    int *buttons = calloc(COMMAND_BOARD_BUTTON_COUNT, sizeof(int));
    int local[COMMAND_BOARD_BUTTON_COUNT] = {
        COMMAND_BUTTON_BUILD_BASE, COMMAND_BUTTON_BUILD_RAM, COMMAND_BUTTON_BUILD_FACTORY, COMMAND_BUTTON_ESCAPE,
        0, 0, 0, 0,
        0, 0, 0, 0};

    for (int i = 0; i < COMMAND_BOARD_BUTTON_COUNT; i++)
    {
        buttons[i] = local[i];
    }
    return buttons;
}