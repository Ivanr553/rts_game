#pragma once

#include "../../../engine/engine.h"
#include "../../models/control_group/control_group.h"
#include "../events/events.h"

void create_control_groups(void);
void initialize_in_game_store(void);
short handle_right_click_unit_interaction(void);

/** Event Handling */
void handle_in_game_event(IN_GAME_EVENT event);
