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

    pm::SpriteSheet sprite_sheet("./resources/sprite_sheet.bmp");
    
    // Create ghost_a
    auto ghost_a = pm::Entity();
    ghost_a.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("ghost_a_default", {0, 32, 16, 16}, 10, {{16, 0}}));
    ghost_a.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("ghost_a_right", {0, 32, 16, 16}, 10, {{16, 0}}));
    ghost_a.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("ghost_a_left", {32, 32, 16, 16}, 10, {{16, 0}}));
    ghost_a.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("ghost_a_down", {96, 32, 16, 16}, 10, {{16, 0}}));
    ghost_a.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("ghost_a_up", {64, 32, 16, 16}, 10, {{16, 0}}));

    auto ghost_b = pm::Entity();
    ghost_b.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("ghost_b_default", {0, 48, 16, 16}, 10, {{16, 0}}));
    ghost_b.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("ghost_b_right", {0, 48, 16, 16}, 10, {{16, 0}}));
    ghost_b.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("ghost_b_left", {32, 48, 16, 16}, 10, {{16, 0}}));
    ghost_b.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("ghost_b_down", {96, 48, 16, 16}, 10, {{16, 0}}));
    ghost_b.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("ghost_b_up", {64, 48, 16, 16}, 10, {{16, 0}}));

    auto ghost_c = pm::Entity();
    ghost_c.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("ghost_c_default", {0, 64, 16, 16}, 10, {{16, 0}}));
    ghost_c.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("ghost_c_right", {0, 64, 16, 16}, 10, {{16, 0}}));
    ghost_c.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("ghost_c_left", {32, 64, 16, 16}, 10, {{16, 0}}));
    ghost_c.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("ghost_c_down", {96, 64, 16, 16}, 10, {{16, 0}}));
    ghost_c.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("ghost_c_up", {64, 64, 16, 16}, 10, {{16, 0}}));

    auto ghost_d = pm::Entity();
    ghost_d.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("ghost_d_default", {0, 80, 16, 16}, 10, {{16, 0}}));
    ghost_d.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("ghost_d_right", {0, 80, 16, 16}, 10, {{16, 0}}));
    ghost_d.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("ghost_d_left", {32, 80, 16, 16}, 10, {{16, 0}}));
    ghost_d.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("ghost_d_down", {96, 80, 16, 16}, 10, {{16, 0}}));
    ghost_d.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("ghost_d_up", {64, 80, 16, 16}, 10, {{16, 0}}));
    
    // Create ghost_a
    auto puckman = pm::Entity();
    puckman.set_direction_sprite(pm::EDirection::Idle, sprite_sheet.new_sprite("puckman_default", {0, 0, 16, 16}, 20, {{16, 0}, {32, 0}, {16, 0}}));
    puckman.set_direction_sprite(pm::EDirection::Right, sprite_sheet.new_sprite("puckman_right", {0, 0, 16, 16}, 20, {{16, 0}, {32, 0}, {16, 0}}));
    puckman.set_direction_sprite(pm::EDirection::Left, sprite_sheet.new_sprite("puckman_left", {0, 0, 16, 16}, 20, {{48, 0}, {64, 0}, {48, 0}}));
    puckman.set_direction_sprite(pm::EDirection::Down, sprite_sheet.new_sprite("puckman_down", {0, 0, 16, 16}, 20, {{112, 0}, {128, 0}, {112, 0}}));
    puckman.set_direction_sprite(pm::EDirection::Up, sprite_sheet.new_sprite("puckcman_up", {0, 0, 16, 16}, 20, {{80, 0}, {96, 0}, {80, 0}}));

    auto puckman_die = sprite_sheet.new_sprite("puckman_die", 
        {0, 0, 16, 16}, 
        20, 
        {{80, 0}, {96, 0}, {0, 16}, {16, 16}, {32, 16}, {48, 16}, {64, 16}, {80, 16}, {96, 16}, {112, 16}, {128, 16}, {144, 16}});
    ghost_a.set_position(10, 20);
    ghost_b.set_position(30, 20);
    ghost_c.set_position(50, 20);
    ghost_d.set_position(70, 20);
    puckman.set_position(100, 20);

    const auto change_dir = [&](pm::EDirection new_dir)
    {
        ghost_a.set_direction(new_dir);
        ghost_b.set_direction(new_dir);
        ghost_c.set_direction(new_dir);
        ghost_d.set_direction(new_dir);
        puckman.set_direction(new_dir);
    };

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
            change_dir(pm::EDirection::Left);
        if (keys[SDL_SCANCODE_RIGHT])
            change_dir(pm::EDirection::Right);
        if (keys[SDL_SCANCODE_UP])
            change_dir(pm::EDirection::Up);
        if (keys[SDL_SCANCODE_DOWN])
            change_dir(pm::EDirection::Down);

        terrain.draw();
        ghost_a.draw();
        ghost_b.draw();
        ghost_c.draw();
        ghost_d.draw();
        puckman.draw();
        puckman_die.draw(SDL_Point(4, 40), 1.0, 1.0);
    }
    return 0;
}
