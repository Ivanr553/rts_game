#include "player.h"

#include "../../state/state.h"

void update_player(void)
{
    Player *player = game_global.game_stores.in_game_store.player;
    Array *population_structure_entries = player->population_structure_entries;

    for (int i = 0; i < population_structure_entries->len; i++)
    {
        Population_Structure_Entry *entry = get_item_from_array(population_structure_entries, i);
        Entity *entity = get_entity_by_id(entry->id);

        if (!entity)
        {
            player->max_population -= entry->pop_increase;
            remove_items_from_array_and_shift(population_structure_entries, i, 1);
            i--;
        }
    }
}

void init_player(int player_slot, FACTION faction)
{
    Player *player = calloc(1, sizeof(Player));
    player->player_slot = player_slot;
    player->faction = faction;
    player->crystal_count = 100;
    player->population = 0;
    player->max_population = 10;
    player->population_structure_entries = create_array(50, sizeof(Population_Structure_Entry));

    player->update_player = update_player;

    game_global.game_stores.in_game_store.player = player;
};

void add_population_structure_id(long id, int pop_increase)
{
    Player *player = game_global.game_stores.in_game_store.player;
    Population_Structure_Entry *entry = calloc(1, sizeof(Population_Structure_Entry));
    entry->id = id;
    entry->pop_increase = pop_increase;
    append_array(player->population_structure_entries, entry);

    player->max_population += pop_increase;
}

short can_spend_resources(int crystal_amount, int other_resource_amount)
{
    Player *player = game_global.game_stores.in_game_store.player;
    if (player->crystal_count < crystal_amount)
    {
        return 0;
    }

    return 1;
}

short spend_resources(int crystal_amount, int other_resource_amount)
{
    Player *player = game_global.game_stores.in_game_store.player;
    if (player->crystal_count < crystal_amount)
    {
        return 0;
    }
    player->crystal_count -= crystal_amount;

    return 1;
}