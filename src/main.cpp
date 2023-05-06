#include <SDL.h>

#include <filesystem>

#include "engine/engine.hpp"
#include "engine/entity.hpp"
#include "game/GhostBase.hpp"
#include "game/puckman.hpp"
#include "engine/terrain.hpp"
#include "game/pacman.hpp"

int main(int argc, char** argv)
{
    pm::Engine::init<pm::Pacman>("PucMan", 700, 600);
        
    const auto terrain = std::make_shared<pm::Terrain>();
    terrain->load_from_file("./resources/level.map");
    const auto terrain_unit_length = terrain->get_unit_length();

    const SDL_Rect tunnel_rect{
            static_cast<int>(terrain->get_width() * pm::Cell::draw_scale * terrain_unit_length),
            0,
            static_cast<int>(pm::Cell::draw_scale * terrain_unit_length),
            static_cast<int>(terrain->get_height() * pm::Cell::draw_scale * terrain_unit_length) };

    auto puckman = std::make_shared<pm::Puckman>(pm::Puckman(terrain));
    puckman->set_velocity(4);

    // Create ghost_a
    auto ghost_a = std::make_shared<pm::Blinky>(terrain, puckman);
    auto ghost_b = std::make_shared<pm::Pinky>(terrain, puckman);
    auto ghost_c = std::make_shared<pm::Inky>(terrain, puckman, ghost_a);
    auto ghost_d = std::make_shared<pm::Clyde>(terrain, puckman);
    std::vector<std::shared_ptr<pm::Entity>> entities{ ghost_a, ghost_b, ghost_c, ghost_d, puckman };
    ghost_a->set_cell_discrete_pos({10, 10});
    ghost_b->set_cell_discrete_pos({10, 10});
    ghost_c->set_cell_discrete_pos({10, 10});
    ghost_d->set_cell_discrete_pos({10, 10});
    puckman->set_cell_discrete_pos({10, 20});

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
            puckman->set_look_direction(pm::Direction::LEFT);
        if (keys[SDL_SCANCODE_RIGHT])
            puckman->set_look_direction(pm::Direction::RIGHT);
        if (keys[SDL_SCANCODE_UP])
            puckman->set_look_direction(pm::Direction::UP);
        if (keys[SDL_SCANCODE_DOWN])
            puckman->set_look_direction(pm::Direction::DOWN);

        for (auto& entity : entities)
            entity->tick();

        terrain->draw();

        for (auto& entity : entities)
            entity->draw();

        // Hide tunnel
        SDL_FillRect(pm::Engine::get().get_surface_handle(), &tunnel_rect, 0);
    }
    return 0;
}
