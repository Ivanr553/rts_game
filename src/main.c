#include <stdio.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "engine/engine.h"
#include "game/init.h"
#include "game/state/loop.h"
#include "generic/generic.h"

static bool should_quit = false;

static void input_handle(void)
{
    const u8 *keyboard_state = SDL_GetKeyboardState(NULL);

    u8 e_key_state = keyboard_state[SDL_SCANCODE_E];
    u8 q_key_state = keyboard_state[SDL_SCANCODE_Q];

    move_camera(
        global.input.up == KS_PRESSED || global.input.up == KS_HELD || global.mouse.y == 0,
        global.input.down == KS_PRESSED || global.input.down == KS_HELD || global.mouse.y == global.render.height - 1,
        global.input.left == KS_PRESSED || global.input.left == KS_HELD || global.mouse.x == 0,
        global.input.right == KS_PRESSED || global.input.right == KS_HELD || global.mouse.x == global.render.width - 1,
        0,
        0,
        q_key_state == KS_HELD || q_key_state == KS_PRESSED,
        e_key_state == KS_HELD || e_key_state == KS_PRESSED);

    if (global.input.escape == KS_PRESSED || global.input.escape == KS_HELD)
    {
        should_quit = true;
    }
};

void test()
{
    mat4x4 result, test1, test2;

    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            test1[row][col] = 1;
            test2[row][col] = 2;
            if (row == 3)
            {
                printf("\n");
            }
        }
    }

    mat4x4_mul(result, test1, test2);

    printf(" -- TEST 1 -- \n");
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            printf(" %f", test1[col][row]);
            if (row == 3)
            {
                printf("\n");
            }
        }
    }
    printf(" -- TEST 1 END -- \n\n");

    printf(" -- TEST 2 -- \n");
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            printf(" %f", test2[col][row]);
            if (row == 3)
            {
                printf("\n");
            }
        }
    }
    printf(" -- TEST 2 END -- \n\n");

    printf(" -- RESULT -- \n");
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            printf(" %f", result[col][row]);
            if (row == 3)
            {
                printf("\n");
            }
        }
    }
    printf(" -- RESULT END -- \n\n");
};

void post_game_stats(void)
{
    printf("\nExiting Game\n\n");
    printf("Deleted entity count: %d\n", removed_entity_count);
    printf("Number of map collisions: %d\n", collisions);

    int null_entities = 0;
    int entities_with_null_render_items = 0;
    Array *entities = global.render.entities;
    for (int i = 0; i < entities->len; i++)
    {
        Entity *entity = get_item_from_array(entities, i);
        if (!entity)
        {
            null_entities++;
        }
        if (!entity->render_item)
        {
            entities_with_null_render_items++;
        }
    }
    printf("Null entities after end: %d\n", null_entities);
    printf("Entities with null render items after end: %d\n", entities_with_null_render_items);

    int null_render_items = 0;
    Shader_Group *shader_groups = global.render.shader_groups;

    for (int s = 0; s < SUPPORTED_SHADER_COUNT; s++)
    {
        Shader_Group shader_group = shader_groups[s];
        Texture_Group *texture_groups = shader_group.texture_groups;

        for (int t = 0; t < SUPPORTED_TEXTURE_COUNT; t++)
        {
            Texture_Group texture_group = texture_groups[t];
            Array *render_items = texture_group.render_items;

            for (int i = 0; i < render_items->len; i++)
            {
                Render_Item *render_item_ptr = get_item_from_array(render_items, i);
                Render_Item render_item = *render_item_ptr;

                if (!render_item_ptr || render_item.cleanup)
                {
                    null_render_items++;
                    continue;
                }
            }
        }
    }
    printf("Null render_items after end: %d\n", null_render_items);
}

int main(void)
{
    printf("\n\n");

    time_init(144);
    config_init();
    render_init();
    initialize_game();

    while (!should_quit)
    {
        // printf("Starting main loop\n");
        time_update();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                should_quit = true;
                break;
            case SDL_MOUSEWHEEL:
                handle_mouse_wheel_event(event.wheel.x, event.wheel.y);
                break;
            default:
                break;
            }
        }

        // printf("Starting input handle\n");
        input_update();
        input_handle();

        game_loop();
        // printf("Finished game loop\n");
        render_begin();

        render();
        // printf("Finished rendering items\n");

        render_end();
        // printf("Render end\n");
        time_update_late();
        // printf("Ending main loop\n");
    }

    post_game_stats();

    return 0;
}
