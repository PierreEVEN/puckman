#include <SDL.h>

#include <filesystem>

#include "engine.hpp"
#include "entity.hpp"
#include "sprite_handler.hpp"
#include "logger.hpp"
#include "terrain.hpp"

SDL_Rect src_bg = {200, 3, 168, 216}; // x,y, w,h (0,0) en haut a gauche
SDL_Rect bg     = {4, 4, 672, 864};   // ici scale x4

SDL_Rect ghost_r = {3, 123, 16, 16};
SDL_Rect ghost_l = {37, 123, 16, 16};
SDL_Rect ghost_d = {105, 123, 16, 16};
SDL_Rect ghost_u = {71, 123, 16, 16};
SDL_Rect ghost   = {34, 34, 32, 32}; // ici scale x2


int main(int argc, char** argv)
{
    pm::Engine::init("PucMan", 700, 900);

    pm::Terrain terrain;
    terrain.load_from_file("./resources/level.map");

    pm::SpriteManager sprite_handler("./resources/pacman_sprites.bmp");

    // Create background
    auto background = sprite_handler.new_sprite("background", 10);
    sprite_handler.add_sub_sprite(background, 200, 3, 168, 216);
    sprite_handler.add_sub_sprite(background, 220, 50, 168, 216);

    // Create ghost_a
    const auto ghost_a_right = sprite_handler.new_sprite("ghost_a_right", 1.0);
    sprite_handler.add_sub_sprite(ghost_a_right, 3, 123, 16, 16);
    sprite_handler.add_sub_sprite(ghost_a_right, 20, 123, 16, 16);
    
    const auto ghost_a_left= sprite_handler.new_sprite("ghost_a_left", 1.0);
    sprite_handler.add_sub_sprite(ghost_a_left, 37, 123, 16, 16);
    sprite_handler.add_sub_sprite(ghost_a_left, 54, 123, 16, 16);
    
    const auto ghost_a_down= sprite_handler.new_sprite("ghost_a_down", 1.0);
    sprite_handler.add_sub_sprite(ghost_a_down, 105, 123, 16, 16);
    sprite_handler.add_sub_sprite(ghost_a_down, 122, 123, 16, 16);

    const auto ghost_a_up= sprite_handler.new_sprite("ghost_a_up", 1.0);
    sprite_handler.add_sub_sprite(ghost_a_up, 71, 123, 16, 16);
    sprite_handler.add_sub_sprite(ghost_a_up, 88, 123, 16, 16);
    
    auto ghost_a = pm::Entity(ghost_a_right);
    ghost_a.set_direction_sprite(pm::EDirection::Right, ghost_a_right);
    ghost_a.set_direction_sprite(pm::EDirection::Left, ghost_a_left);
    ghost_a.set_direction_sprite(pm::EDirection::Down, ghost_a_down);
    ghost_a.set_direction_sprite(pm::EDirection::Up, ghost_a_up);

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

        background.draw(4, 4, 4.0, 4.0);
        //ghost_a.draw();
    }
    return 0;
}
