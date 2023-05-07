#include <SDL.h>

#include <filesystem>

#include "engine/engine.hpp"
#include "game/entity.hpp"
#include "game/player.hpp"
#include "game/pacman_gamemode.hpp"

int main(int argc, char** argv)
{
    pm::Engine::init<pm::PacmanGamemode>("PuckMan", 16 * 21, 16 * 30);
    
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
        auto& player = pm::Engine::get().get_gamemode<pm::PacmanGamemode>().get_player();
        if (keys[SDL_SCANCODE_ESCAPE])
            pm::Engine::get().shutdown();
        if (keys[SDL_SCANCODE_LEFT])
            player->set_look_direction(pm::Direction::LEFT);
        if (keys[SDL_SCANCODE_RIGHT])
            player->set_look_direction(pm::Direction::RIGHT);
        if (keys[SDL_SCANCODE_UP])
            player->set_look_direction(pm::Direction::UP);
        if (keys[SDL_SCANCODE_DOWN])
            player->set_look_direction(pm::Direction::DOWN);

    }
    return 0;
}
