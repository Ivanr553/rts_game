#include "game_entities_internal.h"

#include "../../engine/engine.h"

void update_command_queue(Game_Entity *game_entity)
{
    Array *command_queue = game_entity->command_queue;

    Queued_Command *command = get_item_from_array(command_queue, 0);

    if (!command->has_started)
    {
        switch (command->type)
        {
        case QUEUED_COMMAND_TYPE_BUILD:
            Build_Command *build_command = command->queued_command_data;
            Component_Builder *builder_component = game_entity->builder_component;
            builder_component->building_location[0] = build_command->pos[0];
            builder_component->building_location[1] = build_command->pos[1];
            builder_component->building_location[2] = 0.01;
            builder_component->build_time = build_command->total_build_time;
            builder_component->total_build_time = build_command->total_build_time;
            builder_component->building_entity_type = build_command->type;
            builder_component->state = BUILDER_STATE_COMMUTING;

            game_entity->entity->should_move_to_pos = 0;

            command->has_started = 1;
            break;
        }
    }
    else if (command->is_complete)
    {
        /** Clean up command and start next command (probably just recurse) */

        remove_items_from_array_and_shift(command_queue, 0, 1);

        if (command_queue->len > 0)
        {
            game_entity->update_command_queue(game_entity);
        }
    }
}

Game_Entity *create_game_entity(Entity *entity, int player_slot)
{
    Game_Entity *game_entity = calloc(1, sizeof(Game_Entity));
    game_entity->building_component = NULL;
    game_entity->harvester_component = NULL;
    game_entity->ui_component = NULL;
    game_entity->resource_component = NULL;
    game_entity->command_queue = create_array(20, sizeof(Queued_Command));
    game_entity->update_command_queue = update_command_queue;

    game_entity->entity = entity;
    entity->entity_class = game_entity;

    return game_entity;
}

void command_entity(Game_Entity *game_entity, Queued_Command *command, short should_clear_queue)
{
    if (should_clear_queue)
    {
        printf("Clearing queue not yet implemented\n");
    }
    else
    {
        append_array(game_entity->command_queue, command);
    }
}

void complete_command(Game_Entity *game_entity)
{
    Array *command_queue = game_entity->command_queue;
    Queued_Command *command = get_item_from_array(command_queue, 0);

    command->is_complete = 1;
}