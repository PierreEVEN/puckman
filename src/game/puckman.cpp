#include "puckman.hpp"

#include "pacman.hpp"
#include "engine/engine.hpp"
#include "engine/logger.hpp"
#include "engine/terrain.hpp"

namespace pm
{
Puckman::Puckman(const std::shared_ptr<Terrain>& terrain)
    : Character(terrain)
{
    set_direction_sprite(pm::Direction::NONE, *SpriteSheet::find_sprite_by_name("pacman_default"));
    set_direction_sprite(pm::Direction::RIGHT, *SpriteSheet::find_sprite_by_name("pacman_right"));
    set_direction_sprite(pm::Direction::LEFT, *SpriteSheet::find_sprite_by_name("pacman_left"));
    set_direction_sprite(pm::Direction::DOWN, *SpriteSheet::find_sprite_by_name("pacman_down"));
    set_direction_sprite(pm::Direction::UP, *SpriteSheet::find_sprite_by_name("pacman_up"));

    pause_animation(true);
}

void Puckman::tick()
{
    auto&      terrain             = get_terrain();
    const auto terrain_unit_length = terrain.get_unit_length();
    const int  new_points          = points + terrain.eat(
                                         static_cast<int32_t>(std::round(get_absolute_linear_pos().x() / terrain_unit_length)), static_cast<int32_t>(std::round(get_absolute_linear_pos().y() / terrain_unit_length)));
    Character::tick();
    if (points != new_points)
    {
        points = new_points;
        DEBUG("{} Points", points);
    }
}
}
