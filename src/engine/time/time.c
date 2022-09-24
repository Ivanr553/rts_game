#include <SDL2/SDL.h>

#include "time.h"
#include "../global.h"
#include "../render/render_fonts/render_fonts.h"
#include "../render/render_item/render_item.h"

void time_init(u32 frame_rate)
{
    global.time.frame_rate = frame_rate;
    global.time.frame_delay = 1000.f / frame_rate;
}

void render_frame_data_on_screen(u32 frames)
{
    vec2 size = {0.05, 0.05};
    vec3 pos = {(size[0] * 2) - 1.08, 1.01 - size[1], 0};
    int sprite_sheet_size[2] = {11, 9};
    int sprite_size[2] = {1, 1};

    if (!global.time.frame_rate_entity_id)
    {
        printf("Creating text item\n");
        Entity *frame_rate_entity = create_entity(NULL, pos);
        frame_rate_entity->render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/UI/Text.png");
        frame_rate_entity->render_item->should_ignore_camera = 1;
        frame_rate_entity->offset[0] = 1;
        frame_rate_entity->offset[1] = 1;
        frame_rate_entity->size[0] = size[0];
        frame_rate_entity->size[1] = size[1];

        add_sprite_sheet_data(frame_rate_entity->render_item, sprite_size, sprite_sheet_size);
        init_render_item(frame_rate_entity->render_item, pos, frame_rate_entity->size, NULL, NULL, frame_rate_entity->offset, NULL);
        bind_render_item_data(frame_rate_entity->render_item);
        add_entity(frame_rate_entity);

        global.time.frame_rate_entity_id = frame_rate_entity->id;
    }
    else
    {
        long id = global.time.frame_rate_entity_id;
        Entity *frame_rate_entity = get_entity_by_id(id);
        char frame_data[3];
        sprintf(frame_data, "%d", frames);
        update_text_item(frame_rate_entity->render_item, frame_data, pos, frame_rate_entity->size);
    }
}

void time_update(void)
{
    global.time.now = (f32)SDL_GetTicks();
    global.time.delta = (global.time.now - global.time.last) / 1000.f;
    global.time.last = global.time.now;

    ++global.time.frame_count;

    if (global.time.now - global.time.frame_last >= 1000.f)
    {
        global.time.frame_rate = global.time.frame_count;
        global.time.frame_count = 0;
        global.time.frame_last = global.time.now;
    }

    render_frame_data_on_screen(global.time.frame_rate);
};

void time_update_late(void)
{
    global.time.frame_time = (f32)SDL_GetTicks() - global.time.now;

    if (global.time.frame_delay > global.time.frame_time)
    {
        SDL_Delay(global.time.frame_delay - global.time.frame_time);
    }
};