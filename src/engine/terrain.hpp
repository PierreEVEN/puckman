#pragma once
#include "cell.hpp"
#include <filesystem>

namespace pm
{
class Terrain
{
public:
    Terrain();

    void load_from_file(const std::filesystem::path& path);

    [[nodiscard]] Cell& get_cell(const uint32_t x, const uint32_t y);

    void draw();

    [[nodiscard]] bool is_free(const int32_t x, const int32_t y) const
    {
        //@TODO : Verify if cell is free (also handle negative values)
        return true;
    }

private:
    void update_position_and_walls();
    void update_sprite_handles();

private:
    uint32_t          width;
    uint32_t          height;
    std::vector<Cell> grid;
};
}
