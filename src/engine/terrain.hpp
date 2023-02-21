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

private:
    uint32_t          width;
    uint32_t          height;
    std::vector<Cell> grid;
};
}
