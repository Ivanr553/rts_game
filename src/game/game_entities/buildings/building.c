#include "building.h"

#include "../game_entities_internal.h"
#include "../../../engine/engine.h"
#include "../components/components.h"

char *building = "assets/buildings/Summoning-Circle.png";

void create_arch(float *pos)
{
    /** Initializing data */
    int offset[2] = {3, 1};
    int sprite_sheet_size[2] = {3, 1};
    int sprite_size[2] = {128, 128};
    float size[2] = {3, 3};

    /** Entity */
    Entity *entity = create_entity(building, pos);

    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_VERTICAL_QUAD, SHADER_DEFAULT, "assets/buildings/Summoning-Circle.png");
    append_item_to_render_item(render_item, entity);
    add_sprite_sheet_data(render_item, offset, sprite_size, sprite_sheet_size);
    init_render_item(render_item, pos, size, NULL, NULL);
    bind_render_item_data(render_item);

    add_entity(entity);
}

Game_Entity *create_building(float *pos, float *size, BUILDING_TYPE building_type)
{
    /** Initializing data */
    int offset[2] = {1, 1};
    int sprite_sheet_size[2] = {3, 1};
    int sprite_size[2] = {128, 128};

    /** Entity Class */
    Game_Entity *building = calloc(1, sizeof(Game_Entity));
    add_building_component(building, building_type);

    /** Entity */
    Entity *entity = create_entity(building, pos);
    entity->entity_class_type = ENTITY_CLASS_BUILDING;
    entity->entity_class = building;
    add_collision_data(entity, size[0] / 2);

    /** Render Item */
    Render_Item *render_item = get_render_item(1, RENDER_ITEM_QUAD, SHADER_DEFAULT, "assets/buildings/Summoning-Circle.png");
    append_item_to_render_item(render_item, entity);

    /** Only perform for the first entity */
    if (entity->vbo_pos == 0)
    {
        render_item->is_static_object = 1;

        add_sprite_sheet_data(render_item, offset, sprite_size, sprite_sheet_size);
        init_render_item(render_item, pos, size, NULL, NULL);
        bind_render_item_data(render_item);
    }

    building->entity = entity;

    add_entity(building->entity);

    // float _pos_1[3] = {
    //     pos[0] - 0.05,
    //     pos[1] - 2.2,
    //     1.55,
    // };
    // create_arch(_pos_1);
    // float _pos_2[3] = {
    //     pos[0] - 0.05,
    //     pos[1] + 2.2,
    //     1.55,
    // };
    // create_arch(_pos_2);

    // float _pos_3[3] = {
    //     pos[0] + 2.2,
    //     pos[1],
    //     1.55,
    // };
    // create_arch(_pos_3);

    // float _pos_4[3] = {
    //     pos[0] - 2.2,
    //     pos[1],
    //     1.55,
    // };
    // create_arch(_pos_4);

    return building;
};