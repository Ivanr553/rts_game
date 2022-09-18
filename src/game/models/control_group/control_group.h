#pragma once

#include "../../../engine/engine.h"
#include "../../../generic/generic.h"

#define SELECTED_UNIT_MAX_COUNT 20

typedef enum control_group_slot
{
    CONTROL_SLOT_ONE,
    CONTROL_SLOT_TWO,
    CONTROL_SLOT_THREE,
    CONTROL_SLOT_FOUR,
    CONTROL_SLOT_FIVE,
    CONTROL_SLOT_SIX
} Control_Group_Slot;

typedef struct control_group
{
    Control_Group_Slot slot;
    Array *unit_ids;
} Control_Group;

void control_group_add_unit(Control_Group_Slot slot, long id);