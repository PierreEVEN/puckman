#include <SDL.h>

#include <filesystem>

#include "engine/engine.hpp"
#include "engine/entity.hpp"
#include "game/GhostBase.hpp"
#include "game/puckman.hpp"
#include "engine/sprite_sheet.hpp"
#include "engine/terrain.hpp"

int main(int argc, char** argv)
{
    pm::Engine::init("PucMan", 700, 600);

    pm::SpriteSheet sprite_sheet("./resources/sprite_sheet.bmp");

    // Load terrain sprites
    sprite_sheet.new_sprite("gum", {128, 32, 16, 16}, 10, {});
    sprite_sheet.new_sprite("bigum", {144, 32, 16, 16}, 10, {});
    sprite_sheet.new_sprite("door", {128, 96, 16, 16}, 10, {});
    sprite_sheet.new_sprite("cherry", {128, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet.new_sprite("strawberry", {144, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet.new_sprite("abricot", {160, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet.new_sprite("apple", {176, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet.new_sprite("wtfruit", {192, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet.new_sprite("axe", {208, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet.new_sprite("bell", {224, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet.new_sprite("key", {240, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet.new_sprite("wall_none", {184, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_N", {176, 88, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_E", {176, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_NE", {168, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_W", {168, 88, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_NW", {160, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_EW", {128, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_NEW", {144, 88, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_S", {184, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_NS", {136, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_ES", {144, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_NES", {152, 88, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_WS", {152, 80, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_NWS", {136, 88, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_EWS", {128, 88, 8, 8}, 10, {});
    sprite_sheet.new_sprite("wall_full", {160, 88, 8, 8}, 10, {});
    
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
    puckman->set_direction_sprite(pm::Direction::NONE, sprite_sheet.new_sprite("puckman_default", {0, 0, 16, 16}, 20, {{16, 0}, {32, 0}, {16, 0}}));
    puckman->set_direction_sprite(pm::Direction::RIGHT, sprite_sheet.new_sprite("puckman_right", {0, 0, 16, 16}, 20, {{16, 0}, {32, 0}, {16, 0}}));
    puckman->set_direction_sprite(pm::Direction::LEFT, sprite_sheet.new_sprite("puckman_left", {0, 0, 16, 16}, 20, {{48, 0}, {64, 0}, {48, 0}}));
    puckman->set_direction_sprite(pm::Direction::DOWN, sprite_sheet.new_sprite("puckman_down", {0, 0, 16, 16}, 20, {{112, 0}, {128, 0}, {112, 0}}));
    puckman->set_direction_sprite(pm::Direction::UP, sprite_sheet.new_sprite("puckcman_up", {0, 0, 16, 16}, 20, {{80, 0}, {96, 0}, {80, 0}}));

    // Create ghost_a
    auto ghost_a = std::make_shared<pm::Blinky>(terrain, puckman);
    ghost_a->set_direction_sprite(pm::Direction::NONE, sprite_sheet.new_sprite("ghost_a_default", {0, 32, 16, 16}, 10, {{16, 0}}));
    ghost_a->set_direction_sprite(pm::Direction::RIGHT, sprite_sheet.new_sprite("ghost_a_right", {0, 32, 16, 16}, 10, {{16, 0}}));
    ghost_a->set_direction_sprite(pm::Direction::LEFT, sprite_sheet.new_sprite("ghost_a_left", {32, 32, 16, 16}, 10, {{16, 0}}));
    ghost_a->set_direction_sprite(pm::Direction::DOWN, sprite_sheet.new_sprite("ghost_a_down", {96, 32, 16, 16}, 10, {{16, 0}}));
    ghost_a->set_direction_sprite(pm::Direction::UP, sprite_sheet.new_sprite("ghost_a_up", {64, 32, 16, 16}, 10, {{16, 0}}));
    
    auto ghost_b = std::make_shared<pm::Pinky>(terrain, puckman);
    ghost_b->set_direction_sprite(pm::Direction::NONE, sprite_sheet.new_sprite("ghost_b_default", {0, 48, 16, 16}, 10, {{16, 0}}));
    ghost_b->set_direction_sprite(pm::Direction::RIGHT, sprite_sheet.new_sprite("ghost_b_right", {0, 48, 16, 16}, 10, {{16, 0}}));
    ghost_b->set_direction_sprite(pm::Direction::LEFT, sprite_sheet.new_sprite("ghost_b_left", {32, 48, 16, 16}, 10, {{16, 0}}));
    ghost_b->set_direction_sprite(pm::Direction::DOWN, sprite_sheet.new_sprite("ghost_b_down", {96, 48, 16, 16}, 10, {{16, 0}}));
    ghost_b->set_direction_sprite(pm::Direction::UP, sprite_sheet.new_sprite("ghost_b_up", {64, 48, 16, 16}, 10, {{16, 0}}));
    
    auto ghost_c = std::make_shared<pm::Inky>(terrain, puckman, ghost_a);
    ghost_c->set_direction_sprite(pm::Direction::NONE, sprite_sheet.new_sprite("ghost_c_default", {0, 64, 16, 16}, 10, {{16, 0}}));
    ghost_c->set_direction_sprite(pm::Direction::RIGHT, sprite_sheet.new_sprite("ghost_c_right", {0, 64, 16, 16}, 10, {{16, 0}}));
    ghost_c->set_direction_sprite(pm::Direction::LEFT, sprite_sheet.new_sprite("ghost_c_left", {32, 64, 16, 16}, 10, {{16, 0}}));
    ghost_c->set_direction_sprite(pm::Direction::DOWN, sprite_sheet.new_sprite("ghost_c_down", {96, 64, 16, 16}, 10, {{16, 0}}));
    ghost_c->set_direction_sprite(pm::Direction::UP, sprite_sheet.new_sprite("ghost_c_up", {64, 64, 16, 16}, 10, {{16, 0}}));
    
    auto ghost_d = std::make_shared<pm::Clyde>(terrain, puckman);
    ghost_d->set_direction_sprite(pm::Direction::NONE, sprite_sheet.new_sprite("ghost_d_default", {0, 80, 16, 16}, 10, {{16, 0}}));
    ghost_d->set_direction_sprite(pm::Direction::RIGHT, sprite_sheet.new_sprite("ghost_d_right", {0, 80, 16, 16}, 10, {{16, 0}}));
    ghost_d->set_direction_sprite(pm::Direction::LEFT, sprite_sheet.new_sprite("ghost_d_left", {32, 80, 16, 16}, 10, {{16, 0}}));
    ghost_d->set_direction_sprite(pm::Direction::DOWN, sprite_sheet.new_sprite("ghost_d_down", {96, 80, 16, 16}, 10, {{16, 0}}));
    ghost_d->set_direction_sprite(pm::Direction::UP, sprite_sheet.new_sprite("ghost_d_up", {64, 80, 16, 16}, 10, {{16, 0}}));

    std::vector<std::shared_ptr<pm::Entity>> entities{ ghost_a, ghost_b, ghost_c, ghost_d, puckman };

    auto puckman_die = sprite_sheet.new_sprite("puckman_die",
                                               {0, 0, 16, 16},
                                               20,
                                               {{80, 0}, {96, 0}, {0, 16}, {16, 16}, {32, 16}, {48, 16}, {64, 16}, {80, 16}, {96, 16}, {112, 16}, {128, 16}, {144, 16}});
    ghost_a->set_cell_discrete_pos({10, 10});
    ghost_b->set_cell_discrete_pos({10, 10});
    ghost_c->set_cell_discrete_pos({10, 10});
    ghost_d->set_cell_discrete_pos({10, 10});
    puckman->set_cell_discrete_pos({10, 19});

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
        if (keys[SDL_SCANCODE_P])
            puckman_die.set_paused(true);
        if (keys[SDL_SCANCODE_M])
            puckman_die.set_paused(false);

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
