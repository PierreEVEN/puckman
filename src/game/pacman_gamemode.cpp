#include "pacman_gamemode.hpp"

#include "ghosts.hpp"
#include "ghost_base.hpp"
#include "player.hpp"
#include "engine/entity.hpp"
#include "engine/logger.hpp"
#include "engine/sprite_sheet.hpp"
#include "engine/terrain.hpp"

#include <cassert>
#include <SDL.h>
#include <SDL_surface.h>

namespace pm
{
PacmanGamemode::PacmanGamemode()
    : sprite_sheet(std::make_shared<SpriteSheet>("./resources/sprite_sheet.bmp"))
{
    load_sprites();
    INFO("loaded sprites");

    terrain = std::make_shared<pm::Terrain>();
    terrain->load_from_file("./resources/level.map");
    const auto terrain_unit_length = terrain->get_unit_length();

    player = std::make_shared<pm::Player>(pm::Player(terrain));

    // Create ghost_a
    auto blinky = std::make_shared<pm::Blinky>(terrain, player);
    auto pinky  = std::make_shared<pm::Pinky>(terrain, player);
    auto inky   = std::make_shared<pm::Inky>(terrain, player, blinky);
    auto clyde  = std::make_shared<pm::Clyde>(terrain, player);
    entities    = {blinky, pinky, inky, clyde, player};

    tunnel_rect = {
        static_cast<int>(terrain->get_width() * pm::Cell::draw_scale * terrain_unit_length),
        0,
        static_cast<int>(pm::Cell::draw_scale * terrain_unit_length * 2),
        static_cast<int>(terrain->get_height() * pm::Cell::draw_scale * terrain_unit_length)};

    on_frightened.add_lambda([&]
    {
        INFO("enter frightened mode");
        frightened_timer = 7;
    });

    begin_level();
}

void PacmanGamemode::tick(double delta_seconds)
{
    GamemodeBase::tick(delta_seconds);

    terrain->tick(delta_seconds);

    // Handle AI behaviors. If frightened : don't decrease scatter-chase timer
    if (frightened_timer <= 0)
    {
        scatter_chase_timer -= delta_seconds;
        if (scatter_chase_timer <= 0)
        {
            if (cycle >= 7)
                scatter_chase_timer = DBL_MAX; // infinite
            else
            {
                if (level < 2)
                {
                    scatter_chase_timer = cycle % 2 == 0 ? cycle >= 4 ? 5 : 7 : 20.0;
                }
                else if (level < 5)
                {
                    scatter_chase_timer = cycle % 2 == 0 ? (cycle >= 4 ? (cycle >= 6 ? 1 / 60.0 : 5) : 7) : (cycle >= 5 ? 1033 : 20.0);
                }
                else
                {
                    scatter_chase_timer = cycle % 2 == 0 ? (cycle >= 6 ? 1 / 60.0 : 5) : (cycle >= 5 ? 1037 : 20.0);
                }
            }
            cycle++;
            is_chase = !is_chase;
            if (is_chase)
            {
                assert(scatter_chase_timer >= 20); // should never be bellow 20
                INFO("enter chase mode");
                on_chase.execute();
            }
            else
            {
                assert(scatter_chase_timer <= 7); // should never be above 7
                INFO("enter scatter mode");
                on_scatter.execute();
            }
        }
    }
    else
    {
        frightened_timer -= delta_seconds;
        // back to normal mode
        if (frightened_timer <= 0)
        {
            INFO("end frightened mode");
            if (is_chase)
                on_chase.execute();
            else
                on_scatter.execute();
        }
    }

    if (death_timer > 0)
    {
        const auto last_death_timer = death_timer;
        death_timer -= delta_seconds;
        if (death_timer < 2 && last_death_timer >= 2)
        {
            for (const auto& entity : entities)
                entity->hide(true);
            player->hide(false);
            player->play_death();
        }
        if (death_timer <= 0 && last_death_timer > 0)
        {
            lives--;
            reset_positions();

            if (lives == 0)
            {
                INFO("GAME OVER");
                Engine::get().shutdown();
            }
        }
    }

    if (victory_timer > 0)
    {
        const auto last_victory_timer = victory_timer;
        victory_timer -= delta_seconds;

        if (victory_timer < 3 && last_victory_timer >= 3)
        {
            for (const auto& entity : entities)
                entity->hide(true);
            player->hide(false);
        }

        if (victory_timer <= 0 && last_victory_timer > 0)
        {
            level++;
            INFO("Now on level {}", level);
            begin_level();
        }
    }

    if (spawn_delay > 0)
    {
        const auto last_spawn_delay = spawn_delay;
        spawn_delay -= delta_seconds;

        if (spawn_delay <= 0 && last_spawn_delay > 0)
        {
            player->pause_animation(false);
        }
    }

    for (const auto& entity : entities)
        entity->tick();
}

void PacmanGamemode::draw()
{
    GamemodeBase::draw();
    terrain->draw();
    for (const auto& entity : entities)
        entity->draw();

    for (int32_t i = 0; i < lives; ++i)
    {
        SpriteSheet::find_sprite_by_name("pacman_life")->draw({(2 - i) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    }

    if (level >= 1)
        SpriteSheet::find_sprite_by_name("cherry")->draw({(18) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    if (level >= 2)
        SpriteSheet::find_sprite_by_name("strawberry")->draw({(17) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    if (level >= 4)
        SpriteSheet::find_sprite_by_name("abricot")->draw({(16) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    if (level >= 6)
        SpriteSheet::find_sprite_by_name("apple")->draw({(15) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    if (level >= 8)
        SpriteSheet::find_sprite_by_name("wtfruit")->draw({(14) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    if (level >= 10)
        SpriteSheet::find_sprite_by_name("axe")->draw({(13) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    if (level >= 12)
        SpriteSheet::find_sprite_by_name("bell")->draw({(12) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    if (level > 12)
        SpriteSheet::find_sprite_by_name("key")->draw({(11) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});

    // Hide tunnel
    SDL_FillRect(pm::Engine::get().get_surface_handle(), &tunnel_rect, 0);

    if (victory_timer > 0 && victory_timer < 3)
    {
        if (static_cast<int32_t>(victory_timer * 5) % 2 == 0)
            terrain->set_wall_color(33, 33, 222);
        else
            terrain->set_wall_color(236, 236, 236);
    }

    if (spawn_delay > 0)
        SpriteSheet::find_sprite_by_name("ready")->draw({9 * terrain->get_unit_length(), 15 * terrain->get_unit_length()});
}

void PacmanGamemode::victory()
{
    if (victory_timer > 0)
        return;
    INFO("VICTORY");
    victory_timer = 4;
    player->pause_animation(true);
}

void PacmanGamemode::begin_level()
{
    terrain->reset();
    lives = 3;

    reset_positions();
}

void PacmanGamemode::reset_positions()
{
    victory_timer       = 0;
    spawn_delay         = 2.5;
    death_timer         = 0;
    frightened_timer    = 0;
    scatter_chase_timer = 0;
    is_chase            = true;
    cycle               = 0;

    for (const auto& entity : entities)
        entity->reset();

    player->set_look_direction(Direction::NONE);
    player->pause_animation(true);

    terrain->set_wall_color(33, 33, 222);
}

void PacmanGamemode::death()
{
    if (death_timer <= 0)
    {
        death_timer = 3;
        player->pause_animation(true);
    }
}

void PacmanGamemode::add_points(int32_t added_points)
{
    points += added_points;
    INFO("add {} points; new score is {}", added_points, points);
}
}
