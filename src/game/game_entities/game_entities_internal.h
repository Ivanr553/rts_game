#pragma once

#include <linmath.h>

#include "../../engine/engine.h"
#include "../constants/constants.h"

typedef enum building_type
{
    _BUILDING_TYPE,
    BUILDING_TYPE_BASE
} BUILDING_TYPE;

typedef enum entity_class
{
    _ENTITY_CLASS,
    ENTITY_CLASS_UNIT,
    ENTITY_CLASS_RESOURCE,
    ENTITY_CLASS_COMMAND_BOARD,
    ENTITY_CLASS_UI,
    ENTITY_CLASS_BUILDING
} ENTITY_CLASS;

typedef enum harvesting_state
{
    HARVESTING_STATE_IDLE,
    HARVESTING_STATE_COMMUTING,
    HARVESTING_STATE_HARVESTING,
    HARVESTING_STATE_DEPOSITING
} HARVESTING_STATE;

struct game_entity;

/**
 * Can only carry one resource at a time
 *
 * When harvesting another resource, lose all of the current resource you have
 *
 * Lower harvesting speed = faster harvest (harvest speed in game ticks)
 */
typedef struct component_harvester
{
    HARVESTING_STATE state;
    int harvesting_speed;
    int harvesting_time_left;
    int resource_quantity;
    int max_resource_quantity;
    float harvesting_distance;
    struct game_entity *resource_entity;
    struct game_entity *return_building_entity;
} Component_Harvest;

typedef struct component_building
{
    BUILDING_TYPE building_type;
} Component_Building;

typedef struct component_resource
{
    RESOURCE resource_type;
    int amount;
} Component_Resource;

typedef struct component_ui
{
    void (*on_click)();
} Component_UI;

typedef struct game_entity
{
    Entity *entity;
    short is_selectable;
    short is_selected;

    /** Harvesting Data */
    Component_Harvest *harvester_component;

    /** Building Data */
    Component_Building *building_component;

    /** Resource Data */
    Component_Resource *resource_component;

    /** UI Data*/
    Component_UI *ui_component;

    /** Combat Data */

} Game_Entity;

#define BOUND_ENTITY_SELECTOR "BOUND_ENTITY_SELECTOR"
#define BOUND_ENTITY_BUILDING_PLACEMENT "BOUND_ENTITY_BUILDING_PLACEMENT"

static char DEFAULT_ANIMATION_IDLE[] = "DEFAULT_ANIMATION_IDLE";
static char DEFAULT_ANIMATION_WALKING[] = "DEFAULT_ANIMATION_WALKING";

Game_Entity *create_game_entity(Entity *entity);