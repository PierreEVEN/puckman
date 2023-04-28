#include "puckman.hpp"

#include "engine/logger.hpp"
#include "engine/terrain.hpp"

namespace pm
{
void Puckman::tick()
{
	auto& terrain = get_terrain();
	const auto terrain_unit_length = terrain.get_unit_length();
    const int new_points = points + terrain.eat(
            static_cast<int32_t>(std::round(get_pos_x() / terrain_unit_length)), static_cast<int32_t>(std::round(get_pos_y() / terrain_unit_length)));
    Character::tick();
    if (points != new_points)
    {
        points = new_points;
        DEBUG("{} Points", points);
    }
}
}
