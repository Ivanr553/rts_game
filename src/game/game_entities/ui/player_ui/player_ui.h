#pragma once

#include "../../../../engine/engine.h"

void init_player_resources(void);
void update_player_crystal_text(Entity *entity);
short spend_resources(int crystal_amount, int other_resource_amount);