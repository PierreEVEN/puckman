#include "GhostBase.hpp"

#include "engine/logger.hpp"
#include "engine/pathfinding.hpp"

namespace pm
{
GhostRed::GhostRed(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target, AiMode in_mode)
    : GhostBase(terrain), pathfinder(std::make_shared<PathFinder>(terrain)), target(std::move(in_target)), mode(in_mode)
{
}

void GhostRed::tick()
{
    if (mode == AiMode::Red)
        pathfinder->find_path(get_discrete_pos(), target->get_discrete_pos());
    
    if (mode == AiMode::Pink)
        pathfinder->find_path(get_discrete_pos(), target->get_discrete_pos() * 2 - get_discrete_pos());

    set_look_direction(get_direction(direction_to_vector(pathfinder->direction_to_next_point(get_discrete_pos()))));
    GhostBase::tick();
}

}
