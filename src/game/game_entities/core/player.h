#pragma once

#include "../../../engine/engine.h"

#include "../../constants/constants.h"

typedef struct population_structure_entry
{
    long id;
    int pop_increase;
} Population_Structure_Entry;

typedef struct player
{
    int player_slot;
    FACTION faction;
    int crystal_count;
    int population;
    int max_population;
    Array *population_structure_entries;
    void (*update_player)();
} Player;

void init_player(int player_slot, FACTION faction);
short can_spend_resources(int crystal_amount, int other_resource_amount);
short spend_resources(int crystal_amount, int other_resource_amount);
void add_population_structure_id(long id, int pop_increase);