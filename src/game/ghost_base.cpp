#include "ghost_base.hpp"

#include "pacman_gamemode.hpp"
#include "engine/direction.hpp"
#include "engine/engine.hpp"
#include "engine/logger.hpp"

namespace pm
{
GhostBase::GhostBase(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target)
    : Character(terrain), target(std::move(in_target)), mode(AiMode::Spawned)
{
    frightened_sprite       = *SpriteSheet::find_sprite_by_name("frightened_ghost");
    frightened_sprite_flash = *SpriteSheet::find_sprite_by_name("frightened_ghost_flash");
    eyes_down_sprite        = *SpriteSheet::find_sprite_by_name("eyes_down");
    eyes_left_sprite        = *SpriteSheet::find_sprite_by_name("eyes_left");
    eyes_right_sprite       = *SpriteSheet::find_sprite_by_name("eyes_right");
    eyes_up_sprite          = *SpriteSheet::find_sprite_by_name("eyes_up");
}

GhostBase::~GhostBase()
{
    Engine::get().get_gamemode<PacmanGamemode>().on_frightened.clear_object(this);
    Engine::get().get_gamemode<PacmanGamemode>().on_chase.clear_object(this);
    Engine::get().get_gamemode<PacmanGamemode>().on_scatter.clear_object(this);
}

double GhostBase::compute_speed_percent() const
{
    if (mode == AiMode::GoHome)
        return 2;

    const auto level = Engine::get().get_gamemode<PacmanGamemode>().current_level();
    if (level < 2)
        return mode == AiMode::Frightened ? 0.5 : 0.75;
    if (level < 5)
        return mode == AiMode::Frightened ? 0.55 : 0.85;
    if (level < 21)
        return mode == AiMode::Frightened ? 0.6 : 0.95;

    return 0.95;
}

void GhostBase::tick()
{
    if (mode == AiMode::Spawned)
    {
        Engine::get().get_gamemode<PacmanGamemode>().on_frightened.clear_object(this);
        Engine::get().get_gamemode<PacmanGamemode>().on_chase.clear_object(this);
        Engine::get().get_gamemode<PacmanGamemode>().on_scatter.clear_object(this);
        Engine::get().get_gamemode<PacmanGamemode>().on_frightened.add_object(this, &GhostBase::on_frightened);
        Engine::get().get_gamemode<PacmanGamemode>().on_chase.add_object(this, &GhostBase::on_chase);
        Engine::get().get_gamemode<PacmanGamemode>().on_scatter.add_object(this, &GhostBase::on_scatter);
        set_cell_discrete_pos(home_location());
        mode = AiMode::ExitSpawn;
    }

    go_through_doors = mode == AiMode::GoHome || mode == AiMode::ExitSpawn;
    set_velocity(compute_speed_percent() * 75.75757625 / get_terrain().get_unit_length());

        // Hit player
    if ((get_cell_linear_pos() - target->get_cell_linear_pos()).length() <= 0.25)
    {
        if (mode == AiMode::Frightened)
        {
            mode = AiMode::GoHome;
            set_look_direction(Direction::NONE);
            on_search_new_dir();
            Engine::get().get_gamemode<PacmanGamemode>().add_points(200);
        }
        else if (mode == AiMode::Chase || mode == AiMode::Scatter)
        {
            Engine::get().get_gamemode<PacmanGamemode>().death();
        }
    }

    if (last_cell != get_cell_discrete_pos())
    {
        last_cell = get_cell_discrete_pos();
        on_search_new_dir();
    }
    Character::tick();
}

void GhostBase::reset()
{
    Character::reset();
    set_cell_discrete_pos(home_location());
    hidden = false;
    set_look_direction(Direction::NONE);
    pause_animation(true);
    mode = AiMode::Spawned;
}

void GhostBase::draw()
{
    if (hidden)
        return;
    if (mode == AiMode::Frightened)
    {
        auto s = Cell::draw_scale;
        if (Engine::get().get_gamemode<PacmanGamemode>().frightened_remaining_time() > 2.2)
            frightened_sprite.draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
        else
            frightened_sprite_flash.draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
    }
    else if (mode == AiMode::GoHome)
    {
        auto s = Cell::draw_scale;
        if (get_look_direction().is_down())
            eyes_down_sprite.draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
        else if (get_look_direction().is_left())
            eyes_left_sprite.draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
        else if (get_look_direction().is_right())
            eyes_right_sprite.draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
        else if (get_look_direction().is_up())
            eyes_up_sprite.draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
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
        if ((dir.is_none() || get_look_direction()->dot(*dir) == 0 || dir == get_look_direction()) && get_terrain().is_free(get_cell_discrete_pos() + *dir, go_through_doors))
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
    case AiMode::ExitSpawn:
        target_pos = {10, 10};
        if (target_pos == get_cell_discrete_pos())
        {
            mode             = Engine::get().get_gamemode<PacmanGamemode>().is_chase_time() ? AiMode::Chase : AiMode::Scatter;
            go_through_doors = false;
            set_look_direction(Direction::NONE);
            on_search_new_dir();
            return;
        }
        break;
    case AiMode::GoHome:
        target_pos = home_location();
        if (target_pos == get_cell_discrete_pos())
        {
            mode = AiMode::ExitSpawn;
            on_search_new_dir();
            return;
        }
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
    if (mode == AiMode::GoHome)
        return;
    mode = AiMode::Frightened;
    set_look_direction(get_look_direction().opposite());
    on_search_new_dir();
}

void GhostBase::on_scatter()
{
    if (mode == AiMode::GoHome || mode == AiMode::ExitSpawn)
        return;
    mode = AiMode::Scatter;
    set_look_direction(Direction::NONE);
    on_search_new_dir();
}

void GhostBase::on_chase()
{
    if (mode == AiMode::GoHome || mode == AiMode::ExitSpawn)
        return;
    mode = AiMode::Chase;
    set_look_direction(Direction::NONE);
    on_search_new_dir();
}
}
