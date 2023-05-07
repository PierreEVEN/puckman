#pragma once
#include "engine/engine.hpp"

#include <SDL_rect.h>
#include <engine/event_manager.hpp>

// See engine/event_manager.hpp
DECLARE_DELEGATE_MULTICAST(SimpleEvent)

/*
 * Pacman core gameplay (this is our game mode)
 */

namespace pm
{
class Entity;
class Player;
class Terrain;
class SpriteSheet;

class PacmanGamemode : public GamemodeBase
{
public:
    PacmanGamemode();

    void tick(double delta_seconds) override;
    void draw() override;

    // Get sprite sheet containing all the sprites used in pacman's game
    [[nodiscard]] SpriteSheet& global_spritesheet() const { return *sprite_sheet; }

    // Event called when Ai state is globally changed
    SimpleEvent on_frightened;
    SimpleEvent on_scatter;
    SimpleEvent on_chase;

    [[nodiscard]] const std::shared_ptr<Player>& get_player() const { return player; }
    [[nodiscard]] bool                           is_chase_time() const { return is_chase; }
    [[nodiscard]] int32_t                        current_level() const { return level; }
    [[nodiscard]] double                         frightened_remaining_time() const { return frightened_timer; }
    [[nodiscard]] bool                           stop_movements() const { return death_timer > 0 || victory_timer > 0 || spawn_delay > 0; }

    // When all the pellet have ben eaten. Start victory animation (then go to next level)
    void victory();

    // Kill the player, start death animation
    void death();

    // When eat pellets, ghosts or fruits...
    void add_points(int32_t added_points);

    // Reset everything. Does not reset current levels. (on victory)
    void begin_level();

    // Reset entities (on death). Don't reset levels and pellet states.
    void reset_positions();

private:
    // Implemented in sprite_loader.cpp
    void load_sprites();

    std::shared_ptr<pm::Terrain> terrain;
    std::shared_ptr<SpriteSheet> sprite_sheet;

    int32_t                 level = 1;
    int32_t                 lives = 3;
    std::shared_ptr<Player> player;

    // Player score
    int32_t points = 0;

    // All the entities (like ghosts, including players)
    std::vector<std::shared_ptr<pm::Entity>> entities;

    // Used to render a black rectangle over right tunnel to hide entity when it exit the map.
    SDL_Rect tunnel_rect;

    // scatter-chase current cycle index.
    int  cycle    = 0;
    bool is_chase = true;

    // Ideally we should have implemented a custom class to handle timer a proper way.
    double spawn_delay         = 0;
    double death_timer         = 0.0;
    double frightened_timer    = 0;
    double scatter_chase_timer = 0;
    double victory_timer       = 0;
};
}
