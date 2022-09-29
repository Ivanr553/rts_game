#include "building.h"

#include "../game_entities_internal.h"
#include "../../../engine/engine.h"
#include "../components/components.h"
#include "../../game_map/game_map.h"
#include "../core/player.h"

char *building = "assets/buildings/Summoning-Circle.png";

void create_arch(float *pos)
{
    /** Initializing data */
    int sprite_sheet_size[2] = {3, 1};
    int sprite_size[2] = {128, 128};
    float size[2] = {3, 3};

    /** Entity */
    Entity *entity = create_entity(building, pos);
    entity->offset[0] = 3;
    entity->offset[1] = 1;
    entity->size[0] = size[0];
    entity->size[1] = size[1];

    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_VERTICAL_QUAD, SHADER_DEFAULT, "assets/buildings/Summoning-Circle.png", 1);
    append_item_to_render_item(render_item, entity);
    add_sprite_sheet_data(render_item, sprite_size, sprite_sheet_size);
    init_render_item(render_item, pos, entity->size, NULL, NULL, entity->offset, NULL, 1);
    bind_render_item_data(render_item);

    add_entity(entity);
}

GAME_ENTITY_TYPE get_game_entity_type_from_building_type(BUILDING_TYPE type)
{
    switch (type)
    {
    case BUILDING_TYPE_BASE:
        return GAME_ENTITY_TYPE_BASE;

    case BUILDING_TYPE_FACTORY:
        return GAME_ENTITY_TYPE_FACTORY;

    case BUILDING_TYPE_RAM:
        return GAME_ENTITY_TYPE_RAM;
    }
}

Game_Entity *create_building(float *pos, BUILDING_TYPE building_type)
{
    /** Initializing data */
    int offset[2] = {1, 1};
    int sprite_size[2] = {128, 128};

    Building_Data building_data = get_building_data_by_type(building_type);

    /** Entity Class */
    Game_Entity *building = calloc(1, sizeof(Game_Entity));
    add_building_component(building, building_type, pos, building_data.unit_radius);
    add_selectable_component(building, get_game_entity_type_from_building_type(building_type));

    /** Entity */
    Entity *entity = create_entity(building, pos);
    entity->entity_class_type = ENTITY_CLASS_BUILDING;
    entity->entity_class = building;
    entity->is_fixed_object = 1;
    entity->offset[0] = 1;
    entity->offset[1] = 1;
    entity->size[0] = building_data.size[0];
    entity->size[1] = building_data.size[1];
    add_collision_data(entity, building_data.unit_radius);

    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_DEFAULT, building_data.file_path, 1);
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        add_sprite_sheet_data(render_item, sprite_size, building_data.sprite_sheet_size);
        init_render_item(render_item, pos, entity->size, NULL, NULL, offset, NULL, 1);
        bind_render_item_data(render_item);
    }

    building->entity = entity;

    add_entity(building->entity);

    if(building_data.pop_increase > 0)
    {
        add_population_structure_id(entity->id, building_data.pop_increase);
    }

    return building;
};

Building_Data get_building_data_by_type(BUILDING_TYPE type)
{
    switch (type)
    {
    case BUILDING_TYPE_FACTORY:
    {
        Building_Data building_data = {
            .unit_radius = 1.5,
            .file_path = "assets/buildings/Summoning-Circle.png",
            .can_produce = 1,
            .can_rally = 1,
            .crystal_cost = 150,
            .build_time = 3000,
            .pop_increase = 0};

        building_data.size[0] = 3;
        building_data.size[1] = 3;
        building_data.sprite_sheet_size[0] = 3;
        building_data.sprite_sheet_size[1] = 1;

        return building_data;
    }
    case BUILDING_TYPE_BASE:
    {
        Building_Data building_data = {
            .unit_radius = 2,
            .file_path = "assets/buildings/Summoning-Circle.png",
            .can_produce = 1,
            .can_rally = 1,
            .crystal_cost = 400,
            .build_time = 5000,
            .pop_increase = 0};

        building_data.size[0] = 4;
        building_data.size[1] = 4;
        building_data.sprite_sheet_size[0] = 3;
        building_data.sprite_sheet_size[1] = 1;

        return building_data;
    }

    case BUILDING_TYPE_RAM:
    {
        Building_Data building_data = {
            .unit_radius = 1,
            .file_path = "assets/buildings/RAM.png",
            .can_produce = 0,
            .can_rally = 0,
            .crystal_cost = 100,
            .build_time = 200,
            .pop_increase = 10};

        building_data.size[0] = 2;
        building_data.size[1] = 2;
        building_data.sprite_sheet_size[0] = 1;
        building_data.sprite_sheet_size[1] = 1;

        return building_data;
    }

    default:
        return;
    }
}
