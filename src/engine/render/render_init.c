#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stb/stb_image.h>

#include "../../generic/generic.h"

#include "../util/util.h"
#include "../global.h"
#include "render.h"
#include "render_internal.h"
#include "../io/io.h"
#include "../entity/entity.h"

void render_init(void)
{
    global.render.width = viewportWidth;
    global.render.height = viewportHeight;
    global.render.window = render_init_window(global.render.width, global.render.height);
    global.render.scale = 1;
    global.render.texture_map = create_map(50, 0);
    global.render.tick = 0;

    init_camera();

    init_v2();
};

SDL_Window *render_init_window(u32 width, u32 height)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        ERROR_EXIT("Could not init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow(
        "Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        viewportWidth,
        viewportHeight,
        SDL_WINDOW_OPENGL);

    if (!window)
    {
        ERROR_EXIT("Failed to init window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_CreateContext(window);
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    stbi_set_flip_vertically_on_load(1);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        ERROR_EXIT("Failed to load GL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetWindowGrab(window, SDL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glScissor(0, 0, viewportWidth, viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight);

    puts("OpenGL Loaded");
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    return window;
};

void add_texture_data_to_texture_group(Texture_Group *texture_groups, char *textures[SUPPORTED_TEXTURE_COUNT], int t)
{
    Texture_Group *texture_group = calloc(1, sizeof(Texture_Group));
    texture_group->render_items = create_array(INITIAL_ENTITY_ARRAY_COUNT, sizeof(Render_Item));

    Texture_Data *texture_data = calloc(1, sizeof(Texture_Data));
    texture_data->file_path = textures[t];
    texture_data->texture_location = t;

    Texture_Data *texture_data_in_map_ptr = get_value_from_map(global.render.texture_map, texture_data->file_path);
    if (texture_data_in_map_ptr)
    {
        printf("Found texture in map %s with id: %d\n", texture_data->file_path, *texture_data_in_map_ptr->texture);
        texture_group->texture_id = texture_data_in_map_ptr->texture;
        texture_groups[t] = *texture_group;
        return;
    }

    if (strcmp(EMPTY_TEXTURE, texture_data->file_path) == 0)
    {
        printf("Found empty texture slot\n");
        texture_groups[t] = *texture_group;
        int *empty_texture_id = calloc(1, sizeof(int));
        empty_texture_id[0] = -1;
        texture_data->texture = empty_texture_id;
        set_value_in_map(global.render.texture_map, texture_data->file_path, texture_data);
        return;
    }

    unsigned char *bytes = stbi_load(texture_data->file_path, &texture_data->width, &texture_data->height, &texture_data->color_channels, 0);
    if (!bytes)
    {
        ERROR_EXIT("Unable to load image at file_path: %s\n", texture_data->file_path);
    }

    GLuint *texture = calloc(1, sizeof(GLuint));
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    printf("New textured %s with  id: %d\n", texture_data->file_path, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_data->width, texture_data->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture_data->width, texture_data->height, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    texture_data->texture = texture;
    set_value_in_map(global.render.texture_map, texture_data->file_path, texture_data);

    texture_group->texture_id = texture;
    texture_groups[t] = *texture_group;

    glBindTexture(GL_TEXTURE_2D, 0);
    free(bytes);
}

void init_v2()
{
    global.render.entity_map.free_ids = create_array(50, sizeof(long));
    global.render.entity_map._entity_map = create_map(10000, 1);
    global.render.entities = create_array(300, sizeof(Entity));
    global.render.non_static_entities = create_array(300, sizeof(Entity));
    global.render.ui_entities = create_array(100, sizeof(Entity));

    Shader_Data shader_data_list[SUPPORTED_SHADER_COUNT] = {
        {.vert_path = "./shaders/default.vert",
         .frag_path = "./shaders/default.frag"},
        {.vert_path = "./shaders/light.vert",
         .frag_path = "./shaders/light.frag"},
        {.vert_path = "./shaders/color.vert",
         .frag_path = "./shaders/color.frag"},
    };

    char *textures[SUPPORTED_TEXTURE_COUNT] = {
        EMPTY_TEXTURE,
        "assets/UI/Max-Controls.png",
        "assets/maps/Tile_Set.png",
        "assets/units/MAX.png",
        "assets/units/Alf.png",
        "assets/UI/Text.png",
        "assets/props/Props_Sheet.png",
        "assets/buildings/Summoning-Circle.png",
        "assets/UI/Icons.png",
        "assets/UI/Command-Buttons.png",
        "assets/units/SPIKE.png",
    };

    Shader_Group *shader_groups = calloc(SUPPORTED_SHADER_COUNT, sizeof(Shader_Group));

    for (int s = 0; s < SUPPORTED_SHADER_COUNT; s++)
    {
        Shader_Group shader_group;

        Shader_Data shader_data = shader_data_list[s];
        u32 shader_id = render_shader_create(shader_data.vert_path, shader_data.frag_path);
        shader_group.shader_id = shader_id;

        Texture_Group *texture_groups = calloc(SUPPORTED_TEXTURE_COUNT, sizeof(Texture_Group));
        for (int t = 0; t < SUPPORTED_TEXTURE_COUNT; t++)
        {
            add_texture_data_to_texture_group(texture_groups, textures, t);
        }

        shader_group.texture_groups = texture_groups;
        shader_groups[s] = shader_group;
    }

    global.render.shader_groups = shader_groups;
}