#include "components.h"

#include "../../../engine/engine.h"
#include "../game_entities_internal.h"

void update_building_component(Game_Entity *game_entity)
{
    Component_Building *building_component = game_entity->building_component;
    if (building_component->queued_builds->len > 0)
    {
        Queued_Build *queued_build = get_item_from_array(building_component->queued_builds, 0);
        
        queued_build->build_time--;

        if (queued_build->build_time == 0)
        {
            queued_build->create_unit(game_entity);
            remove_items_from_array_and_shift(building_component->queued_builds, 0, 1);
        }
    }
}

void add_building_component(Game_Entity *game_entity, BUILDING_TYPE building_type, float *pos, float unit_radius)
{
    Component_Building *building_component = calloc(1, sizeof(Component_Building));
    building_component->building_type = building_type;
    building_component->queued_builds = create_array(DEFAULT_QUEUED_BUILD_AMOUNT, sizeof(Queued_Build));
    building_component->update_building_component = update_building_component;
    building_component->rally_point = calloc(3, sizeof(float));
    building_component->rally_point[0] = pos[0] + unit_radius;
    building_component->rally_point[1] = pos[1] + unit_radius;
    building_component->rally_point[2] = 0;

    game_entity->building_component = building_component;
};