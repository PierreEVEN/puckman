#pragma once

#include "ghost_base.hpp"

/*
 * This is the behavior specialization for Blinky, Pinky, Inky and Clyde
 */

namespace pm
{	
class Blinky : public GhostBase
{
public:
    Blinky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);

    [[nodiscard]] Vector2I target_player() const override;
    [[nodiscard]] Vector2I scatter_target() const override;
    [[nodiscard]] Vector2I home_location() const override;

private:
};


class Pinky : public GhostBase
{
public:
    Pinky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);

    [[nodiscard]] Vector2I target_player() const override;
    [[nodiscard]] Vector2I scatter_target() const override;
    [[nodiscard]] Vector2I home_location() const override;

private:
};



class Inky : public GhostBase
{
public:
    Inky(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target, std::shared_ptr<Character> blinky_ref);

    [[nodiscard]] Vector2I target_player() const override;
    [[nodiscard]] Vector2I scatter_target() const override;
    [[nodiscard]] Vector2I home_location() const override;

private:
    std::shared_ptr<Character> blinky;
};



class Clyde : public GhostBase
{
public:
    Clyde(const std::shared_ptr<Terrain>& terrain, std::shared_ptr<Character> in_target);

    [[nodiscard]] Vector2I target_player() const override;
    [[nodiscard]] Vector2I scatter_target() const override;
    [[nodiscard]] Vector2I home_location() const override;

private:
};
}