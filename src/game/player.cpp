#include "player.hpp"

#include "pacman_gamemode.hpp"
#include "engine/engine.hpp"
#include "engine/logger.hpp"
#include "engine/terrain.hpp"

namespace pm
{
Player::Player(const std::shared_ptr<Terrain>& terrain)
    : Character(terrain)
{
    set_direction_sprite(pm::Direction::NONE, *SpriteSheet::find_sprite_by_name("pacman_default"));
    set_direction_sprite(pm::Direction::RIGHT, *SpriteSheet::find_sprite_by_name("pacman_right"));
    set_direction_sprite(pm::Direction::LEFT, *SpriteSheet::find_sprite_by_name("pacman_left"));
    set_direction_sprite(pm::Direction::DOWN, *SpriteSheet::find_sprite_by_name("pacman_down"));
    set_direction_sprite(pm::Direction::UP, *SpriteSheet::find_sprite_by_name("pacman_up"));

    pause_animation(true);

    death_sprite = *SpriteSheet::find_sprite_by_name("pacman_die");
}

void Player::tick()
{
    const auto level      = Engine::get().get_gamemode<PacmanGamemode>().current_level();
    const auto frightened = Engine::get().get_gamemode<PacmanGamemode>().current_level() > 0;

    const auto speed_multiplier = frightened ? (level == 1 ? 0.9 : level < 5 ? 0.95 : 1) : (level == 1 ? 0.8 : level < 5 ? 0.9 : level < 21 ? 1 : 0.9);

    get_terrain().eat(get_cell_discrete_pos());

    set_velocity(75.75757625 / get_terrain().get_unit_length() * speed_multiplier);
    Character::tick();
}

void Player::draw()
{
    if (hidden)
        return;

    if (should_play_death)
    {
        auto s = Cell::draw_scale;
        death_sprite.draw(get_absolute_discrete_pos() * static_cast<int32_t>(s), s, s);
    }
    else
        Character::draw();
}

void Player::play_death()
{
    death_sprite.reset_timer();
    should_play_death = true;
}

void Player::reset()
{
    Character::reset();
    set_cell_discrete_pos({10, 20});
    hidden            = false;
    should_play_death = false;
    set_look_direction(Direction::NONE);
    pause_animation(true);
}

void Player::set_look_direction(const Direction new_direction)
{
    if (Engine::get().is_init() && Engine::get().get_gamemode<PacmanGamemode>().stop_movements())
        return;

    Character::set_look_direction(new_direction);
}
}
