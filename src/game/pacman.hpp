#pragma once
#include "engine/engine.hpp"

#include <SDL_rect.h>
#include <engine/event_manager.hpp>

DECLARE_DELEGATE_MULTICAST(SimpleEvent)

namespace pm
{
class Entity;
class Puckman;
class Terrain;
class SpriteSheet;

class Pacman : public GamemodeBase
{
public:
    Pacman();

    void tick(double delta_seconds) override;
    void draw() override;

    [[nodiscard]] SpriteSheet& global_spritesheet() const { return *sprite_sheet; }

    SimpleEvent on_frightened;
    SimpleEvent on_scatter;
    SimpleEvent on_chase;

    [[nodiscard]] const std::shared_ptr<Puckman>& get_player() const { return player; }
    [[nodiscard]] bool                            is_chase_time() const { return is_chase; }
    [[nodiscard]] int32_t                         current_level() const { return level; }
    [[nodiscard]] double                          frightened_remaining_time() const { return frightened_timer; }
    [[nodiscard]] bool                            stop_movements() const { return death_timer > 0; }

    void death();

private:
    void load_sprites();

    std::shared_ptr<pm::Terrain>             terrain;
    std::shared_ptr<SpriteSheet>             sprite_sheet;
    int32_t                                  level = 1;
    int32_t                                  lives = 3;
    std::shared_ptr<Puckman>                 player;
    std::vector<std::shared_ptr<pm::Entity>> entities;
    SDL_Rect                                 tunnel_rect;
    double                                   frightened_timer    = 0;
    double                                   scatter_chase_timer = 0;
    bool                                     is_chase            = true;
    int                                      cycle               = 0;
    double                                   death_timer         = 0.0;
};
}
