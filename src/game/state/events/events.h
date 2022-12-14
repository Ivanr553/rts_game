#pragma once

#include <linmath.h>

typedef enum global_event
{
    _GLOBAL_EVENT,
    GLOBAL_EVENT_MOUSE_RIGHT_BUTTON,
    GLOBAL_EVENT_MOUSE_LEFT_BUTTON,
    GLOBAL_EVENT_MOUSE_LEFT_BUTTON_DRAG_RELEASE,
    GLOBAL_EVENT_KEYBOARD_1,
    GLOBAL_EVENT_KEYBOARD_2,
} Global_Event;

typedef enum in_game_event
{
    _IN_GAME_EVENT,

    /** DEBUG ONLY*/
    IN_GAME_EVENT_DEBUG_DELETE,

    IN_GAME_EVENT_ESCAPE,

    /** Command Button Events */
    /** MUST MAINTAIN ORDER */
    IN_GAME_EVENT_COMMAND_BUTTON_1,
    IN_GAME_EVENT_COMMAND_BUTTON_2,
    IN_GAME_EVENT_COMMAND_BUTTON_3,
    IN_GAME_EVENT_COMMAND_BUTTON_4,
    IN_GAME_EVENT_COMMAND_BUTTON_5,
    IN_GAME_EVENT_COMMAND_BUTTON_6,
    IN_GAME_EVENT_COMMAND_BUTTON_7,
    IN_GAME_EVENT_COMMAND_BUTTON_8,
    IN_GAME_EVENT_COMMAND_BUTTON_9,
    IN_GAME_EVENT_COMMAND_BUTTON_10,
    IN_GAME_EVENT_COMMAND_BUTTON_11,
    IN_GAME_EVENT_COMMAND_BUTTON_12
} IN_GAME_EVENT;

void handle_events(void);
void handle_global_event(Global_Event event);

short handle_ui_element_click(float x, float y);

void handle_right_click_in_game(vec3 pos);