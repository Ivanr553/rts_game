#pragma once

#include "../../engine/engine.h"

#include "../models/models.h"
#include "../game_map/game_map.h"
#include "../../generic/generic.h"
#include "stores/in_game_store.h"
#include "../game_entities/game_entities.h"

typedef enum game_state
{
    LOADING,
    MENUS,
    IN_GAME
} Game_State;

typedef enum game_state_store
{
    STORE_IN_GAME,
    STORE_UI,
    STORE_SETTINGS
} Game_State_Store;

typedef struct in_game_store
{
    /** Metadata */
    Game_State_Store store_type;

    /** Player */
    Player *player;

    /** Unit Control */
    Control_Group *control_groups;
    Control_Group_Slot selected_control_group;
    Array *selected_units_by_id;

    /** Building Placement */
    long building_selection_entity_id;
    short is_placing_building;
    BUILDING_TYPE building_being_placed;

    /** Map */
    Game_Map *map;

    /** UI */
    Array *selected_unit_icon_ids;
    Array *command_button_ids;
    Array *command_board_info_ids;
} In_Game_Store;

typedef struct game_stores
{
    In_Game_Store in_game_store;
} Game_Stores;

typedef struct game_global
{
    Game_State state;
    Entity *mouse_ray;
    int press_count;
    Game_Stores game_stores;
} Game_Global;

extern Game_Global game_global;

void handle_event(Game_State_Store store, void *event);