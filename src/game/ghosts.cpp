#include "ghosts.hpp"

namespace pm
{
Blinky::Blinky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target)
    : GhostBase(terrain, in_target)
{
    set_direction_sprite(Direction::NONE, *SpriteSheet::find_sprite_by_name("ghost_a_default"));
    set_direction_sprite(Direction::RIGHT, *SpriteSheet::find_sprite_by_name("ghost_a_right"));
    set_direction_sprite(Direction::LEFT, *SpriteSheet::find_sprite_by_name("ghost_a_left"));
    set_direction_sprite(Direction::DOWN, *SpriteSheet::find_sprite_by_name("ghost_a_down"));
    set_direction_sprite(Direction::UP, *SpriteSheet::find_sprite_by_name("ghost_a_up"));
}

Vector2I Blinky::target_player() const
{
    return target->get_cell_discrete_pos();
}

Vector2I Blinky::scatter_target() const
{
    // Top right corner
    return {static_cast<int32_t>(get_terrain().get_width()) - 2, -3};
}

Vector2I Blinky::home_location() const
{
    return {10, 10};
}

Pinky::Pinky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target)
    : GhostBase(terrain, in_target)
{
    set_direction_sprite(Direction::NONE, *SpriteSheet::find_sprite_by_name("ghost_b_default"));
    set_direction_sprite(Direction::RIGHT, *SpriteSheet::find_sprite_by_name("ghost_b_right"));
    set_direction_sprite(Direction::LEFT, *SpriteSheet::find_sprite_by_name("ghost_b_left"));
    set_direction_sprite(Direction::DOWN, *SpriteSheet::find_sprite_by_name("ghost_b_down"));
    set_direction_sprite(Direction::UP, *SpriteSheet::find_sprite_by_name("ghost_b_up"));
}

Vector2I Pinky::target_player() const
{
    return target->get_cell_discrete_pos() + dir_with_overflow_error(target->get_look_direction()) * 4;
}

Vector2I Pinky::scatter_target() const
{
    // Top left corner
    return {2, -3};
}

Vector2I Pinky::home_location() const
{
    return {10, 12};
}

Inky::Inky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target, std::shared_ptr<Character> blinky_ref)
    : GhostBase(terrain, std::move(in_target)), blinky(std::move(blinky_ref))
{
    set_direction_sprite(Direction::NONE, *SpriteSheet::find_sprite_by_name("ghost_c_default"));
    set_direction_sprite(Direction::RIGHT, *SpriteSheet::find_sprite_by_name("ghost_c_right"));
    set_direction_sprite(Direction::LEFT, *SpriteSheet::find_sprite_by_name("ghost_c_left"));
    set_direction_sprite(Direction::DOWN, *SpriteSheet::find_sprite_by_name("ghost_c_down"));
    set_direction_sprite(Direction::UP, *SpriteSheet::find_sprite_by_name("ghost_c_up"));
}

Vector2I Inky::target_player() const
{
    const auto middle_point = target->get_cell_discrete_pos() + dir_with_overflow_error(target->get_look_direction()) * 2;
    return blinky->get_cell_discrete_pos() + (middle_point - blinky->get_cell_discrete_pos()) * 2;
}

Vector2I Inky::scatter_target() const
{
    // Bottom right corner
    return {static_cast<int32_t>(get_terrain().get_width()), static_cast<int32_t>(get_terrain().get_height()) + 1};
}

Vector2I Inky::home_location() const
{
    return {11, 12};
}

Clyde::Clyde(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target)
    : GhostBase(terrain, in_target)
{
    set_direction_sprite(Direction::NONE, *SpriteSheet::find_sprite_by_name("ghost_d_default"));
    set_direction_sprite(Direction::RIGHT, *SpriteSheet::find_sprite_by_name("ghost_d_right"));
    set_direction_sprite(Direction::LEFT, *SpriteSheet::find_sprite_by_name("ghost_d_left"));
    set_direction_sprite(Direction::DOWN, *SpriteSheet::find_sprite_by_name("ghost_d_down"));
    set_direction_sprite(Direction::UP, *SpriteSheet::find_sprite_by_name("ghost_d_up"));
}

Vector2I Clyde::target_player() const
{
    const auto player_distance = (get_cell_discrete_pos() - target->get_cell_discrete_pos()).length();

    if (player_distance > 8)
        return scatter_target();

    return target->get_cell_discrete_pos();
}

Vector2I Clyde::scatter_target() const
{
    // Bottom left corner
    return {0, static_cast<int32_t>(get_terrain().get_height()) + 1};
}

Vector2I Clyde::home_location() const
{
    return {9, 12};
}
}
