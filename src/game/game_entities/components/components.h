#pragma once

#include "../../../engine/engine.h"
#include "../game_entities_internal.h"

/** Selection */
void create_selection(Entity *entity_to_bind_to);
void update_selection(Entity *select_entity);

/** Harvesting */
void add_harvester_component(Game_Entity *unit_entity, int harvesting_speed, float harvesting_distance);
void add_resource_harvesting_data(Component_Harvest *harvester_component, RESOURCE resource);
void start_harvesting(Game_Entity *unit_entity, Game_Entity *resource_entity);
void handle_harvesting(Game_Entity *unit_entity);
void stop_harvesting(Game_Entity *unit_entity);

/** Building */
void add_building_component(Game_Entity *unit_entity, BUILDING_TYPE building_type);

/** Resource */
void add_resource_component(Game_Entity *unit_entity, RESOURCE resource_type, int amount);

/** UI */
void add_ui_component(Game_Entity *game_entity, void (*on_click)());

/** Selection */
void add_selectable_component(Game_Entity *game_entity, UNIT_TYPE unit_type);