#include "components.h"

#include "../../../engine/engine.h"
#include "../game_entities_internal.h"

Weapon_Data *get_weapon_data(WEAPON_TYPE weapon_type)
{
    Weapon_Data *weapon_data = calloc(1, sizeof(Weapon_Data));
    switch (weapon_type)
    {
    case WEAPON_TYPE_MELEE:
        weapon_data->attack_speed = 1;
        weapon_data->damage = 10;
        weapon_data->range = 1;
        break;

    default:
        return NULL;
    }

    return weapon_data;
}

void update_combat_component(Game_Entity *game_entity)
{
    Component_Combat *combat_component = game_entity->combat_component;

    if (combat_component->is_attacking)
    {
        Game_Entity *target_entity = combat_component->target_entity;

        if (!target_entity || target_entity->marked_for_deletion)
        {
            printf("Target is null or dead\n");
            combat_component->target_entity = NULL;
            combat_component->is_attacking = 0;
            return;
        }

        // do range calculations and move if needed

        Component_Combat *target_entity_combat_component = target_entity->combat_component;
        if (!target_entity_combat_component)
        {
            printf("Attacking an invalid target! No combat data on target\n");
            combat_component->target_entity = NULL;
            combat_component->is_attacking = 0;
            return;
        }

        Weapon_Data *weapon_data = get_weapon_data(combat_component->weapon_type_1);

        /** In range to attack */
        combat_component->weapon_1_attack_counter++;
        int counter_value_to_attack = 60 / weapon_data->attack_speed;

        if (combat_component->weapon_1_attack_counter < counter_value_to_attack)
        {
            printf("Waiting to hit\n");
            return;
        }

        /** Attacking */
        target_entity_combat_component->health -= weapon_data->damage - target_entity_combat_component->armor;
        combat_component->weapon_1_attack_counter = 0;
    }

    if (combat_component->health <= 0)
    {
        printf("Killed unit\n");
        game_entity->marked_for_deletion = 1;
    }
};

void add_combat_component(Game_Entity *game_entity, int max_health, int max_energy, int armor, WEAPON_TYPE weapon_type_1, WEAPON_TYPE weapon_type_2)
{
    Component_Combat *combat_component = calloc(1, sizeof(Component_Combat));
    combat_component->max_health = max_health;
    combat_component->health = max_health;
    combat_component->armor = armor;
    combat_component->max_energy = max_energy;
    combat_component->energy = max_energy;
    combat_component->weapon_type_1 = weapon_type_1;
    combat_component->weapon_type_2 = weapon_type_2;
    combat_component->update_combat_component = update_combat_component;

    game_entity->combat_component = combat_component;
}