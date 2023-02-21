#include <SDL.h>

#include <filesystem>

#include "engine/engine.hpp"
#include "engine/entity.hpp"
#include "engine/sprite_sheet.hpp"
#include "engine/terrain.hpp"

int main(int argc, char** argv)
{
    pm::Engine::init("PucMan", 700, 900);

    pm::Terrain terrain;
    terrain.load_from_file("./resources/level.map");

    pm::SpriteSheet sprite_sheet("./resources/pacman_sprites.bmp");

    // Create background
    auto background = sprite_sheet.new_sprite("background", {200, 3, 168, 216});

    // Create ghost_a
    auto ghost_a = pm::Entity();
    ghost_a.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("ghost_a_default", {3, 123, 16, 16}, 10, {{17, 0}}));
    ghost_a.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("ghost_a_right", {3, 123, 16, 16}, 10, {{17, 0}}));
    ghost_a.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("ghost_a_left", {37, 123, 16, 16}, 10, {{17, 0}}));
    ghost_a.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("ghost_a_down", {105, 123, 16, 16}, 10, {{17, 0}}));
    ghost_a.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("ghost_a_up", {71, 123, 16, 16}, 10, {{17, 0}}));
    
    auto ghost_b = pm::Entity();
    ghost_b.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("ghost_b_default", {3, 142, 16, 16}, 10, {{17, 0}}));
    ghost_b.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("ghost_b_right", {3, 142, 16, 16}, 10, {{17, 0}}));
    ghost_b.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("ghost_b_left", {37, 142, 16, 16}, 10, {{17, 0}}));
    ghost_b.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("ghost_b_down", {105, 142, 16, 16}, 10, {{17, 0}}));
    ghost_b.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("ghost_b_up", {71, 142, 16, 16}, 10, {{17, 0}}));
    
    auto ghost_c = pm::Entity();
    ghost_c.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("ghost_c_default", {3, 160, 16, 16}, 10, {{17, 0}}));
    ghost_c.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("ghost_c_right", {3, 160, 16, 16}, 10, {{17, 0}}));
    ghost_c.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("ghost_c_left", {37, 160, 16, 16}, 10, {{17, 0}}));
    ghost_c.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("ghost_c_down", {105, 160, 16, 16}, 10, {{17, 0}}));
    ghost_c.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("ghost_c_up", {71, 160, 16, 16}, 10, {{17, 0}}));
    
    auto ghost_d = pm::Entity();
    ghost_d.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("ghost_d_default", {3, 178, 16, 16}, 10, {{17, 0}}));
    ghost_d.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("ghost_d_right", {3, 178, 16, 16}, 10, {{17, 0}}));
    ghost_d.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("ghost_d_left", {37, 178, 16, 16}, 10, {{17, 0}}));
    ghost_d.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("ghost_d_down", {105, 178, 16, 16}, 10, {{17, 0}}));
    ghost_d.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("ghost_d_up", {71, 178, 16, 16}, 10, {{17, 0}}));

    // Create ghost_a
    auto puckman = pm::Entity();
    puckman.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("puckman_default", {4, 90, 14, 14}, 10, {{17, 0}, {31, 0}, {17, 0}}));
    puckman.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("puckman_right", {3, 90, 16, 16}, 10, {{17, 0}, {34, 0}, {17, 0}}));
    puckman.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("puckman_left", {37, 90, 16, 16}, 10, {{17, 0}, {34, 0}, {17, 0}}));
    puckman.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("puckman_down", {105, 90, 16, 16}, 10, {{17, 0}, {34, 0}, {17, 0}}));
    puckman.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("puckcman_up", {71, 90, 16, 16}, 10, {{17, 0}, {34, 0}, {17, 0}}));

    puckman.set_position(100, 20);

    while (pm::Engine::get().next_frame())
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                pm::Engine::get().shutdown();
                break;
            default:
                break;
            }
        }

        // Gestion du clavier        
        int          nbk;
        const Uint8* keys = SDL_GetKeyboardState(&nbk);
        if (keys[SDL_SCANCODE_ESCAPE])
            pm::Engine::get().shutdown();
        if (keys[SDL_SCANCODE_LEFT])
            puts("LEFT");
        if (keys[SDL_SCANCODE_RIGHT])
            puts("RIGHT");

        background.draw(SDL_Point(4, 4), 4.0, 4.0);
        ghost_a.draw();
        ghost_b.draw();
        ghost_c.draw();
        ghost_d.draw();
        puckman.draw();
    }
    return 0;
}
