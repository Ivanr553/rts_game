#pragma once

#include "../../../engine/engine.h"

#define SHOW_DRAG_ON_MAP 0

void init_drag_select(void);
void update_drag_select(Entity *entity);
float *get_drag_select_bounds(float bounds[6]);