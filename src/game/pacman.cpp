#include "pacman.hpp"

#include "ghost_base.hpp"
#include "puckman.hpp"
#include "engine/entity.hpp"
#include "engine/logger.hpp"
#include "engine/sprite_sheet.hpp"
#include "engine/terrain.hpp"

#include <SDL_surface.h>

namespace pm
{
Pacman::Pacman()
    : sprite_sheet(std::make_shared<SpriteSheet>("./resources/sprite_sheet.bmp"))
{
    load_sprites();
    INFO("loaded sprites");

    terrain = std::make_shared<pm::Terrain>();
    terrain->load_from_file("./resources/level.map");
    terrain->set_wall_color(0, 0, 255);
    const auto terrain_unit_length = terrain->get_unit_length();

    player = std::make_shared<pm::Puckman>(pm::Puckman(terrain));

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

    for (const auto& entity : entities)
        entity->reset();
}

void Pacman::tick(double delta_seconds)
{
    GamemodeBase::tick(delta_seconds);

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
            player->hide(true);
            for (const auto& entity : entities)
                entity->reset();
            lives--;
            if (lives == 0)
            {
                exit(EXIT_SUCCESS);
            }
        }
    }

    for (const auto& entity : entities)
        entity->tick();
}

void Pacman::draw()
{
    GamemodeBase::draw();
    terrain->draw();
    for (const auto& entity : entities)
        entity->draw();

    for (int32_t i = 0; i < static_cast<int32_t>(lives); ++i)
    {
        SpriteSheet::find_sprite_by_name("pacman_life")->draw({(2 - i) * terrain->get_unit_length(), (static_cast<int32_t>(terrain->get_height())) * terrain->get_unit_length()});
    }

    // Hide tunnel
    SDL_FillRect(pm::Engine::get().get_surface_handle(), &tunnel_rect, 0);
}

void Pacman::death()
{
    if (death_timer <= 0)
    {
        death_timer = 3;
        player->pause_animation(true);
    }
}
}
