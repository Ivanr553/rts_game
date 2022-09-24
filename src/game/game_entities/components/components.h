#pragma once

#include "../../../engine/engine.h"
#include "../game_entities_internal.h"

#define DEFAULT_QUEUED_BUILD_AMOUNT 6

/** Selection */
void create_selection(Entity *entity_to_bind_to);
void update_selection(Entity *select_entity);

/** Harvesting */
void add_harvester_component(Game_Entity *unit_entity, int harvesting_speed, float harvesting_distance);
void update_harvester_component(Game_Entity *unit_entity);
void add_resource_harvesting_data(Component_Harvest *harvester_component, RESOURCE resource);
void start_harvesting(Game_Entity *unit_entity, Game_Entity *resource_entity);
void stop_harvesting(Game_Entity *unit_entity);

/** Building */
void add_building_component(Game_Entity *unit_entity, BUILDING_TYPE building_type);

/** Resource */
void add_resource_component(Game_Entity *unit_entity, RESOURCE resource_type, int amount);
void resource_start_harvesting(Game_Entity *game_entity, long entity_id);
Game_Entity *find_nearest_free_resource(Game_Entity *game_entity);

/** UI */
void add_ui_component(Game_Entity *game_entity, UI_TYPE type, Game_Entity *parent_entity, void (*on_click)(Game_Entity *), void (*update_ui_entity)(Game_Entity *));

/** Selection */
void add_selectable_component(Game_Entity *game_entity, GAME_ENTITY_TYPE unit_type);

/** Combat */
void add_combat_component(Game_Entity *game_entity, int max_health, int max_energy, int armor, WEAPON_TYPE weapon_type_1, WEAPON_TYPE weapon_type_2);

/** Builder */
void add_builder_component(Game_Entity *game_entity);