#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <json/json.h>

#include "../engine/engine.h"

#include "constants/constants.h"

#include "state/game_global.h"
#include "../generic/generic.h"
#include "models/models.h"
#include "game_map/game_map.h"
#include "game_entities/game_entities.h"

void initialize_state(void)
{
    game_global.state = IN_GAME;

    init_drag_select();

    /** INITIALIZING STORES */
    initialize_in_game_store();
};

void initialize_map(void)
{
    Game_Map *map = calloc(1, sizeof(Game_Map));
    init_game_map(map, "tilesets/Lost-Ruins.json");

    global.camera.position[0] = map->start_x;
    global.camera.position[1] = map->start_y - MAP_TILE_OFFSET_FOR_CAMERA;
}

void initialize_game(void)
{
    initialize_state();
    init_player(0, FACTION_ALCHEMISTS);
    initialize_map();
    init_command_board();
    init_player_resources();

    int test_worker_count = 10;
    time_t t;
    srand((unsigned)time(&t));

    for (int i = 0; i < test_worker_count; i++)
    {

        float x_deviation = game_global.game_stores.in_game_store.map->start_x - 1;
        float y_deviation = game_global.game_stores.in_game_store.map->start_y - 0.5 - (float)((float)i / (float)10);

        GAME_ENTITY_TYPE unit_type = i % 2 ? GAME_ENTITY_TYPE_MAX : GAME_ENTITY_TYPE_ALF;

        Game_Entity *worker = create_worker((vec3){x_deviation, y_deviation, DEFAULT_UNIT_Z}, GAME_ENTITY_TYPE_MAX);
    }

    create_building((vec3){game_global.game_stores.in_game_store.map->start_x, game_global.game_stores.in_game_store.map->start_y, 0.05}, BUILDING_TYPE_BASE);
};
