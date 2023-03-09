#pragma once
#include "character.hpp"

namespace pm
{
class PathFinder;

class Ghost: public Character
{
public:
    explicit Ghost(const std::shared_ptr<Terrain>& terrain)
        : Character(terrain)
    {
    }

    void draw() override;
};

class GhostCustom: public Ghost
{
public:
    explicit GhostCustom(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);

    void draw() override;
    
private:
    const std::shared_ptr<PathFinder> pathfinder;
    std::shared_ptr<Character> target;
};

}
