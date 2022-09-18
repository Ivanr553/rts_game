#include "player.h"

#include "../../state/state.h"

void init_player(int player_slot, FACTION faction)
{
    Player *player = calloc(1, sizeof(Player));
    player->player_slot = player_slot;
    player->faction = faction;
    player->crystal_count = 0;
    player->population = 0;

    game_global.game_stores.in_game_store.player = player;
};