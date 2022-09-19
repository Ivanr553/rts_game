#pragma once

#include "../../../../engine/engine.h"
#include "../../game_entities_internal.h"

typedef struct entity_ui_element
{
    Entity *ui_entity;
    void (*on_click)();
} Entity_Ui_Element;

typedef enum command_button
{
    _COMMAND_BUTTON,
    COMMAND_BUTTON_MOVE,
    COMMAND_BUTTON_ATTACK,
    COMMAND_BUTTON_HOLD,
    COMMAND_BUTTON_STOP,
    COMMAND_BUTTON_BUILD
} COMMAND_BUTTON;

/** Command Board */
void init_command_board(void);

/** Command Board Buttons*/
void init_command_board_buttons(int x, int y, int offset[2], COMMAND_BUTTON button);
void *get_on_click(COMMAND_BUTTON button);
void command_button_build(void);

/** Selection Icons */
void generate_selection_icons(void);
void select_unit(Game_Entity *game_entity);