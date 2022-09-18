#pragma once

#include "../../../engine/engine.h"

#include "../../constants/constants.h"

typedef struct player
{
    int player_slot;
    FACTION faction;
    int crystal_count;
    int population;
} Player;

void init_player(int player_slot, FACTION faction);