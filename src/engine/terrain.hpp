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

    [[nodiscard]] bool is_free(const int32_t x, const int32_t y, bool is_door_free=false) const
    {
        if (x < 0 || y < 0 || uint32_t(x) >= width || uint32_t(y) >= height)
            return false;

        auto cell_type = grid[x + y * width].get_type();
        return cell_type != ECellType::Wall && (is_door_free || cell_type != ECellType::Door);
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
