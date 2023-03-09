#include "ghost.hpp"

#include "engine/logger.hpp"
#include "engine/pathfinding.hpp"

namespace pm
{
void Ghost::draw()
{
    if (rand() % 100 == 0)
        set_look_direction((EDirection)(rand() % 4 + 1));

    Character::draw();
}

GhostCustom::GhostCustom(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target)
    : Ghost(terrain), pathfinder(std::make_shared<PathFinder>(terrain)), target(std::move(in_target))
{
}

void GhostCustom::draw()
{
    pathfinder->find_path(get_discrete_pos(), target->get_discrete_pos());

    set_look_direction(get_direction(pathfinder->direction_to_next_point(get_discrete_pos())));
    Character::draw();
}

}
