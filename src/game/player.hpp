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
private:
    bool         should_play_death = false;
    SpriteHandle death_sprite;
};
}
