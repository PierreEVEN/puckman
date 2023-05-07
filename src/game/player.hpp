#pragma once
#include "character.hpp"

namespace pm
{
class Terrain;

class Player : public Character
{
public:
    explicit Player(const std::shared_ptr<Terrain>& terrain);

    void tick() override;
    void draw() override;

    // Only visual : play death animation
    void play_death();

    void reset() override;

    // Implemented to prevent inputs to be sent while the game is not started
    void set_look_direction(const Direction new_direction) override;

private:
    bool         should_play_death = false;
    SpriteHandle death_sprite;
};
}
