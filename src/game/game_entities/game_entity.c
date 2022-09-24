#include "game_entities_internal.h"

#include "../../engine/engine.h"

Game_Entity *create_game_entity(Entity *entity, int player_slot)
{
    Game_Entity *game_entity = calloc(1, sizeof(Game_Entity));
    game_entity->building_component = NULL;
    game_entity->harvester_component = NULL;
    game_entity->ui_component = NULL;
    game_entity->resource_component = NULL;
    game_entity->command_queue = create_array(20, sizeof(Queued_Command));

    game_entity->entity = entity;
    entity->entity_class = game_entity;

    return game_entity;
}