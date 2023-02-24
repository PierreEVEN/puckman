#pragma once
#include "character.hpp"

namespace pm
{
class Ghost: public Character
{
public:
    explicit Ghost(const std::shared_ptr<Terrain>& terrain)
        : Character(terrain)
    {
    }

    void draw() override;
};

}
