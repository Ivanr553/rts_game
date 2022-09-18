#pragma once

typedef enum global_event {
    GLOBAL_EVENT_MOUSE_RIGHT_BUTTON,
    GLOBAL_EVENT_MOUSE_LEFT_BUTTON,
    GLOBAL_EVENT_MOUSE_LEFT_BUTTON_DRAG_RELEASE,
    GLOBAL_EVENT_KEYBOARD_1,
    GLOBAL_EVENT_KEYBOARD_2,
} Global_Event;

void handle_events(void);
void handle_global_event(Global_Event event);

short handle_ui_element_click(float x, float y);