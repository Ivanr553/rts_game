#pragma once

#include "../../../../engine/engine.h"
#include "../../game_entities_internal.h"

#define COMMABD_BOARD_WIDTH 1.5

#define COMMAND_BOARD_BUTTON_ROW_COUNT 4
#define COMMAND_BOARD_BUTTON_COUNT 12
#define COMMAND_BOARD_INFO_COUNT 10

#define COMMAND_BOARD_BUTTON_SIZE 0.07655

#define COMMAND_BOARD_BUTTON_START_X 0.661
#define COMMAND_BOARD_BUTTON_START_Y -0.0575
#define COMMAND_BOARD_BUTTON_GAP 0.0073

typedef struct entity_ui_element
{
    Entity *ui_entity;
    void (*on_click)();
} Entity_Ui_Element;

typedef enum command_button
{
    _COMMAND_BUTTON,
    COMMAND_BUTTON_ESCAPE,
    COMMAND_BUTTON_MOVE,
    COMMAND_BUTTON_ATTACK,
    COMMAND_BUTTON_HOLD,
    COMMAND_BUTTON_STOP,

    /** Buildings */
    COMMAND_BUTTON_BUILD,
    COMMAND_BUTTON_BUILD_BASE,
    COMMAND_BUTTON_BUILD_FACTORY,
    COMMAND_BUTTON_BUILD_RAM,
    
    /** Units */
    COMMAND_BUTTON_BUILD_MAX,
    COMMAND_BUTTON_BUILD_SPIKE
} COMMAND_BUTTON;

/** Command Board */
void init_command_board(void);

/** Command Board Buttons*/
void show_command_board_buttons_by_selected_unit(void);
void *get_on_click(COMMAND_BUTTON button);
void command_button_build(void);
void generate_command_buttons(int *buttons);
int *get_command_buttons_by_unit_type(GAME_ENTITY_TYPE type);
int *get_building_command_buttons(void);

/** Selection Icons */
void generate_selection_icons(void);
void select_unit(Game_Entity *game_entity);

/** Command Board Info */
void show_command_board_info(Game_Entity *game_entity);