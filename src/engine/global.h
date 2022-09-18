#pragma once

#include "render/render.h"
#include "io/config.h"
#include "time/time.h"
#include "camera/camera.h"

typedef struct global
{
    Render_State render;
    Config_State config;
    Input_State input;
    Mouse_State mouse;
    Time_State time;
    Camera camera;
} Global;

extern Global global;