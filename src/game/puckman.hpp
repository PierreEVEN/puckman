#pragma once
#include "character.hpp"

namespace pm
{
class Terrain;

class Puckman : public Character
{
public:
    explicit Puckman(const std::shared_ptr<Terrain>& terrain);

    void tick() override;

    void draw() override;

    void play_death();

    void reset() override;
private:
    bool         should_play_death = false;
    SpriteHandle death_sprite;
};
}
