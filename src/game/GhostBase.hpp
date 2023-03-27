#pragma once
#include "character.hpp"

namespace pm
{
class PathFinder;

class GhostBase: public Character
{
public:
    explicit GhostBase(const std::shared_ptr<Terrain>& terrain)
        : Character(terrain)
    {
    }
};

enum class AiMode
{
    Any,
    Red,
    Pink,

};

class GhostRed: public GhostBase
{
public:
    explicit GhostRed(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target, AiMode in_mode);

    void draw() override;
    
private:
    const std::shared_ptr<PathFinder> pathfinder;
    std::shared_ptr<Character> target;
    AiMode mode;
};


}
