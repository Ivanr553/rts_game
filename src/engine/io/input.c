#include <linmath.h>

#include "input.h"

#include "../global.h"
#include "../types.h"
#include "../util/constants.h"

void input_init(void)
{
    global.mouse.x = 0;
    global.mouse.y = 0;
    global.mouse.pos_gl[0] = 0;
    global.mouse.pos_gl[1] = 0;
}

static void update_key_state(u8 current_state, Key_State *key_state)
{
    // printf("Current state: %d | New state: %d\n", current_state, *key_state);
    if (current_state)
    {
        if (*key_state == KS_HELD || *key_state == KS_PRESSED)
        {
            *key_state = KS_HELD;
        }
        else
        {
            *key_state = KS_PRESSED;
        }
    }
    else
    {
        *key_state = KS_UNPRESSED;
    }
};

void input_update(void)
{
    const u8 *keyboard_state = SDL_GetKeyboardState(NULL);

    // Keyboard inputs
    update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_LEFT]], &global.input.left);
    update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_RIGHT]], &global.input.right);
    update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_UP]], &global.input.up);
    update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_DOWN]], &global.input.down);
    update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_ESCAPE]], &global.input.escape);

    // Modifiers
    // Key_State ctrl;
    update_key_state(keyboard_state[SDL_SCANCODE_LCTRL], &global.input.ctrl);
    // update_key_state(keyboard_state[SDL_SCANCODE_RCTRL], &global.input.ctrl);
    // Key_State shift;
    update_key_state(keyboard_state[SDL_SCANCODE_LSHIFT], &global.input.shift);
    // update_key_state(keyboard_state[SDL_SCANCODE_RSHIFT], &global.input.shift);
    // Key_State alt;
    update_key_state(keyboard_state[SDL_SCANCODE_LALT], &global.input.alt);
    // update_key_state(keyboard_state[SDL_SCANCODE_RALT], &global.input.alt);
    // Key_State space;
    update_key_state(keyboard_state[SDL_SCANCODE_SPACE], &global.input.space);
    // Key_State tab;
    update_key_state(keyboard_state[SDL_SCANCODE_TAB], &global.input.tab);

    // Num Keys
    update_key_state(keyboard_state[SDL_SCANCODE_1], &global.input.one);
    update_key_state(keyboard_state[SDL_SCANCODE_2], &global.input.two);

    // Char Keys
    update_key_state(keyboard_state[SDL_SCANCODE_A], &global.input.a);
    update_key_state(keyboard_state[SDL_SCANCODE_B], &global.input.b);

    // Mouse inputs
    u32 buttons = SDL_GetMouseState(&global.mouse.x, &global.mouse.y);

    global.mouse.pos_gl[0] = (float)((double)(global.mouse.x * 2) / (double)viewportWidth) - 1;
    global.mouse.pos_gl[1] = (float)1 - ((double)(global.mouse.y * 2) / (double)viewportHeight);
    global.mouse.pos_gl[2] = 0;

    update_key_state((buttons & SDL_BUTTON_LMASK) != 0, &global.mouse.leftButton);
    update_key_state((buttons & SDL_BUTTON_RMASK) != 0, &global.mouse.rightButton);

    if (global.mouse.drag_state == DRAG_STATE_RELEASED)
    {
        global.mouse.drag_start_x = 0;
        global.mouse.drag_start_y = 0;
        global.mouse.drag_state = _DRAG_STATE;
    }

    if (global.mouse.leftButton == KS_PRESSED)
    {
        global.mouse.drag_start_x = global.mouse.x;
        global.mouse.drag_start_y = global.mouse.y;
        global.mouse.drag_state = DRAG_STATE_DRAGGING;
    }
    else if (global.mouse.leftButton == KS_UNPRESSED)
    {
        if (global.mouse.drag_state == DRAG_STATE_DRAGGING)
        {
            global.mouse.drag_state = DRAG_STATE_RELEASED;
        }
    }
};

void handle_mouse_wheel_event(int x, int y)
{
    move_camera(y > 0, y < 0, 0, 0, y<0, y> 0, 0, 0);
};