#pragma once

#include "../../../../engine/engine.h"

typedef enum building
{
    _BUILDING,
    BUILDING_SUMMONING_CIRCLE
} BUILDING;

void show_building_selection(int building_width, int building_height);
void hide_building_selection(void);
void update_build_selection(Entity *entity);