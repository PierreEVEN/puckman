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

    int points = 0;
};
}
