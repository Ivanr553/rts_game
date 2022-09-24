#pragma once

#include <linmath.h>

#include "../../engine/engine.h"
#include "../constants/constants.h"

typedef enum GAME_ENTITY_TYPE
{
    _GAME_ENTITY_TYPE,
    GAME_ENTITY_TYPE_MAX,
    GAME_ENTITY_TYPE_ALF,

    /** Buildings */
    GAME_ENTITY_TYPE_BASE,
} GAME_ENTITY_TYPE;

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

typedef enum weapon_type
{
    _WEAPON_TYPE,
    WEAPON_TYPE_MELEE,
    WEAPON_TYPE_BULLET,
    WEAPON_TYPE_MISSILE,
    WEAPON_TYPE_ENERGY,
    WEAPON_TYPE_SIEGE
} WEAPON_TYPE;

typedef enum ui_type
{
    _UI_TYPE,

    UI_TYPE_COMMAND_BOARD_BUTTON_1,
    UI_TYPE_COMMAND_BOARD_BUTTON_2,
    UI_TYPE_COMMAND_BOARD_BUTTON_3,
    UI_TYPE_COMMAND_BOARD_BUTTON_4,
    UI_TYPE_COMMAND_BOARD_BUTTON_5,
    UI_TYPE_COMMAND_BOARD_BUTTON_6,
    UI_TYPE_COMMAND_BOARD_BUTTON_7,
    UI_TYPE_COMMAND_BOARD_BUTTON_8,
    UI_TYPE_COMMAND_BOARD_BUTTON_9,
    UI_TYPE_COMMAND_BOARD_BUTTON_10,
    UI_TYPE_COMMAND_BOARD_BUTTON_11,
    UI_TYPE_COMMAND_BOARD_BUTTON_12,
    UI_TYPE_COMMAND_BOARD_BUTTON_13,
    UI_TYPE_COMMAND_BOARD_BUTTON_14,
    UI_TYPE_COMMAND_BOARD_BUTTON_15,
    UI_TYPE_COMMAND_BOARD_BUTTON_16,
    UI_TYPE_COMMAND_BOARD_BUTTON_17,
    UI_TYPE_COMMAND_BOARD_BUTTON_18,
    UI_TYPE_COMMAND_BOARD_BUTTON_19,
    UI_TYPE_COMMAND_BOARD_BUTTON_20,

    UI_TYPE_COMMAND_BOARD_QUEUE_1,
    UI_TYPE_COMMAND_BOARD_QUEUE_2,
    UI_TYPE_COMMAND_BOARD_QUEUE_3,
    UI_TYPE_COMMAND_BOARD_QUEUE_4,
    UI_TYPE_COMMAND_BOARD_QUEUE_5,
} UI_TYPE;

typedef enum queued_command_type
{
    _QUEUED_COMMAND_TYPE,
    QUEUED_COMMAND_TYPE_MOVE,
    QUEUED_COMMAND_TYPE_ATTACK,
    QUEUED_COMMAND_TYPE_HARVEST,
    QUEUED_COMMAND_TYPE_BUILD,
} QUEUED_COMMAND_TYPE;

typedef struct weapon_data
{
    float range;
    int damage;
    /**
     *  1 = once per 60 ticks (once per second at max frame rate)
     *  2 = twice per 60 ticks (twice per second)
     */
    float attack_speed;
} Weapon_Data;

struct game_entity;

typedef struct build_command
{
    vec3 pos;
    int total_build_time;
    GAME_ENTITY_TYPE type;
} Build_Command;

typedef struct queued_command
{
    QUEUED_COMMAND_TYPE type;
    void *queued_command_data;
} Queued_Command;

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
    void (*update_harvester_component)(struct game_entity *);
} Component_Harvest;

typedef struct queued_build
{
    int build_time;
    int total_build_time;
    int icon_offset[2];
    void (*create_unit)(struct game_entity *);
} Queued_Build;

typedef struct component_building
{
    BUILDING_TYPE building_type;
    Array *queued_builds;
    void (*update_building_component)(struct game_entity *);
} Component_Building;

typedef struct component_resource
{
    RESOURCE resource_type;
    int amount;
    int recently_harvested_count;
    long recently_harvested_by_id;
} Component_Resource;

typedef struct component_ui
{
    /** When needing to interact with a game entity */
    struct game_entity *game_entity_pointer;
    UI_TYPE type;
    void (*on_click)(struct game_entity *);
    void (*update_ui_entity)(struct game_entity *);
} Component_UI;

typedef struct component_selectable
{
    short is_selected;
} Component_Selectable;

typedef struct component_combat
{
    int health;
    int max_health;
    int armor;
    int energy;
    int max_energy;
    WEAPON_TYPE weapon_type_1;
    int weapon_1_attack_counter;
    WEAPON_TYPE weapon_type_2;
    int weapon_2_attack_counter;

    short is_attacking;
    struct game_entity *target_entity;
    float attack_command_location[3];

    void (*update_combat_component)(struct game_entity *);
} Component_Combat;

typedef struct component_builder
{
    int total_build_time;
    int build_time;
    int build_speed;
    BUILDING_TYPE building_entity_type;
    void (*update_builder_component)(struct game_entity *);
} Component_Builder;

typedef struct game_entity
{
    Entity *entity;

    /** Metadata */
    GAME_ENTITY_TYPE unit_type;
    int player_slot;
    short marked_for_deletion;

    /** Command Queue */
    Array *command_queue;

    /** Harvesting Data */
    Component_Harvest *harvester_component;

    /** Building Data */
    Component_Building *building_component;

    /** Resource Data */
    Component_Resource *resource_component;

    /** UI Data*/
    Component_UI *ui_component;

    /** Selection Data */
    Component_Selectable *selectable_component;

    /** Combat Data */
    Component_Combat *combat_component;

    /** Builder Data */
    Component_Builder *builder_component;
} Game_Entity;

#define BOUND_ENTITY_SELECTOR "BOUND_ENTITY_SELECTOR"
#define BOUND_ENTITY_BUILDING_PLACEMENT "BOUND_ENTITY_BUILDING_PLACEMENT"

static char DEFAULT_ANIMATION_IDLE[] = "DEFAULT_ANIMATION_IDLE";
static char DEFAULT_ANIMATION_WALKING[] = "DEFAULT_ANIMATION_WALKING";

Game_Entity *create_game_entity(Entity *entity, int player_slot);