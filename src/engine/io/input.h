#pragma once

typedef enum input_key
{
    INPUT_KEY_LEFT,
    INPUT_KEY_RIGHT,
    INPUT_KEY_UP,
    INPUT_KEY_DOWN,
    INPUT_KEY_ESCAPE
} Input_Key;

typedef enum key_state
{
    KS_UNPRESSED,
    KS_PRESSED,
    KS_HELD
} Key_State;

typedef enum drag_state
{
    _DRAG_STATE,
    DRAG_STATE_DRAGGING,
    DRAG_STATE_RELEASED
} DRAG_STATE;

typedef struct mouse_state
{
    int x;
    int drag_start_x;
    int y;
    int drag_start_y;
    int time_since_last_click;
    short did_double_click;
    DRAG_STATE drag_state;
    vec3 pos_gl;
    Key_State leftButton;
    Key_State rightButton;
} Mouse_State;

typedef struct input_state
{
    Key_State left;
    Key_State right;
    Key_State up;
    Key_State down;
    Key_State escape;
    Key_State ctrl;
    Key_State shift;
    Key_State alt;
    Key_State space;
    Key_State tab;

    /** Function Keys */
    Key_State f3;
    Key_State f10;

    /** Num keys */
    Key_State one;
    Key_State two;

    /** Char */
    Key_State a;
    Key_State b;

    Key_State q;
    Key_State w;
    Key_State e;
    Key_State r;
    Key_State t;

    Key_State s;
    Key_State d;
    Key_State f;
    Key_State g;

    Key_State z;
    Key_State x;
    Key_State c;
    Key_State v;

} Input_State;

void input_init(void);
void input_update(void);
void handle_mouse_wheel_event(int x, int y);