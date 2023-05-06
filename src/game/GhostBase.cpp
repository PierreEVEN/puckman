#include "GhostBase.hpp"

#include "pacman.hpp"
#include "engine/direction.hpp"
#include "engine/engine.hpp"
#include "engine/logger.hpp"
#include "engine/pathfinding.hpp"

namespace pm
{
GhostBase::GhostBase(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target)
    : Character(terrain), pathfinder(std::make_shared<PathFinder>(terrain)), target(std::move(in_target)), mode(AiMode::Spawned)
{
    frightened_sprite = *SpriteSheet::find_sprite_by_name("frightened_ghost");


    Engine::get().get_gamemode<Pacman>().on_eat_big_gum.add_object(this, &GhostBase::on_frightened);
}

GhostBase::~GhostBase()
{
    Engine::get().get_gamemode<Pacman>().on_eat_big_gum.clear_object(this);
}

void GhostBase::tick()
{
    if (mode == AiMode::Spawned)
    {
        set_cell_discrete_pos(home_location());
        mode = AiMode::Chase;
    }

    if (last_cell != get_cell_discrete_pos())
    {
        last_cell = get_cell_discrete_pos();
        on_search_new_dir();
    }
    Character::tick();
}

void GhostBase::draw()
{
    if (mode == AiMode::Frightened)
    {
        auto s = Cell::draw_scale;
        frightened_sprite.draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
    }
    else
        Character::draw();
}

void GhostBase::on_search_new_dir()
{
    // Compute available direction for next cell
    std::vector<Direction> available_directions;
    for (const auto& dir : Direction::enumerate())
    {
        // Test if the given direction is ok and free
        if ((dir.is_none() || get_look_direction()->dot(*dir) == 0 || dir == get_look_direction()) && get_terrain().is_free(get_cell_discrete_pos() + *dir))
            available_directions.emplace_back(dir);
    }

    // Ghost don't change direction outside intersections
    if (available_directions.empty() || (available_directions.size() == 1 && available_directions.front() == get_look_direction()))
    {
        return;
    }

    Vector2I target_pos = get_cell_discrete_pos();

    switch (mode)
    {
    case AiMode::Chase:
        target_pos = target_player();
        break;
    case AiMode::Scatter:
        target_pos = scatter_target();
        break;
    case AiMode::Frightened:
        target_pos = Vector2I{Engine::get().random_int(0, get_terrain().get_width()), Engine::get().random_int(0, get_terrain().get_height())};
        break;
    default: ;
    }

    // Compute next closest cell to target dir
    Direction min_direction = {};
    auto      min_distance  = DBL_MAX;

    for (const auto& dir : available_directions)
    {
        const auto distance = (get_cell_discrete_pos() + *dir - target_pos).cast<Vector2D>().length();
        if (distance <= min_distance)
        {
            min_distance  = distance;
            min_direction = dir;
        }
    }

    set_look_direction(min_direction);
}

void GhostBase::on_frightened()
{
    mode = AiMode::Frightened;
    set_look_direction(get_look_direction().opposite());
}

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

    if (player_distance < 8)
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
