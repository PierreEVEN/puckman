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

    void play_death();

    void reset() override;
    void set_look_direction(const Direction new_direction) override;

private:
    bool         should_play_death = false;
    SpriteHandle death_sprite;
};
}
