#include "terrain.hpp"
#include "logger.hpp"
#include <fstream>

namespace pm
{
Terrain::Terrain()
    : width(0), height(0)
{
}

void Terrain::load_from_file(const std::filesystem::path& path)
{
    width  = 0;
    height = 0;
    grid.clear();

    std::vector<std::vector<char>> lines;

    std::ifstream input(path);

    if (!input)
    {
        ERROR("Failed to read map file '{}'", path.string());
        return;
    }

    int64_t new_width = -1;

    std::string line;
    while (std::getline(input, line))
    {
        if (new_width == -1)
            new_width = static_cast<int64_t>(line.length());
        else if (new_width != static_cast<int64_t>(line.length()))
        {
            ERROR("Wrong line length : {}", line);
            return;
        }

        lines.emplace_back(std::vector<char>{line.begin(), line.end()});
    }
    if (new_width < 0)
    {
        ERROR("File %s is empty", path.string());
        return;
    }

    width  = static_cast<uint32_t>(new_width);
    height = static_cast<uint32_t>(lines.size());

    grid.reserve(static_cast<size_t>(width) * static_cast<size_t>(height));
    for (const auto& s_line : lines)
        for (const auto& cell : s_line)
            // TODO : Thomas : Cell creation here, maybe find a better approach.
            grid.emplace_back(Cell::from_char(cell));
    INFO("Successfully loaded {}", path.string());
}

Cell& Terrain::get_cell(const uint32_t x, const uint32_t y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        FATAL("Cannot read grid cell {}/{}", x, y);

    return grid[x + y * width];
}

void Terrain::draw()
{
    for (auto& cell : grid)
        cell.draw();
}
}
