#pragma once

#include <SDL2/SDL.h>
#include "../types.h"
#include "render.h"
#include "../io/io.h"
#include "../../generic/generic.h"

SDL_Window *render_init_window(u32 width, u32 height);
u32 render_shader_create(const char *path_vert, const char *path_frag);
void init_v2(void);