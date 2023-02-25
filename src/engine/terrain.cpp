#include "terrain.hpp"
#include "engine.hpp"
#include "logger.hpp"
#include <fstream>
#include <SDL_surface.h>

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
            grid.emplace_back(Cell::from_char(cell));

    update_position_and_walls();

    update_sprite_handles();

    create_wall_cache_surface();

    INFO("Successfully loaded {}", path.string());
}

int Terrain::eat(const int32_t x, const int32_t y)
{
    if (x < 0 || y < 0 || uint32_t(x) >= width || uint32_t(y) >= height)
        return 0;

    auto& cell = get_cell(x, y);

    switch (cell.get_type())
    {
    case ECellType::Gum:
        cell.update_type(ECellType::Void);
        //@TODO : Don't rebuild all sprites each time
        update_sprite_handles();
        return 1;
    case ECellType::Item:
        cell.update_type(ECellType::Void);
        //@TODO : Don't rebuild all sprites each time
        update_sprite_handles();
        return 100;
    case ECellType::BiGum:
        cell.update_type(ECellType::Void);
        //@TODO : Don't rebuild all sprites each time
        update_sprite_handles();
        return 10;
    default:
        break;
    }
    return 0;
}

void Terrain::update_position_and_walls()
{
    for (uint32_t y = 0; y < height; ++y)
        for (uint32_t x = 0; x < width; ++x)
        {
            Cell& cell = grid[y * width + x];

            cell.set_pos({int(x), int(y)});

            if (cell.get_type() == ECellType::Wall)
            {
                WallMask mask = 0;
                if (y > 0 && grid[(y - 1) * width + x].get_type() == ECellType::Wall)
                    mask |= WALL_MASK_NORTH;
                if (x > 0 && grid[y * width + (x - 1)].get_type() == ECellType::Wall)
                    mask |= WALL_MASK_WEST;
                if (x + 1 < width && grid[y * width + (x + 1)].get_type() == ECellType::Wall)
                    mask |= WALL_MASK_EAST;
                if (y + 1 < height && grid[(y + 1) * width + x].get_type() == ECellType::Wall)
                    mask |= WALL_MASK_SOUTH;
                cell.set_wall(mask);
            }
        }
}

void Terrain::update_sprite_handles()
{
    const SpriteHandle null_handle;

    auto get_sprite_handle = [null_handle](const std::string& name)
    {
        return SpriteSheet::find_sprite_by_name(name).value_or(null_handle);
    };

    std::unordered_map<ECellType, SpriteHandle> map_cell_type = {
        {ECellType::Void, null_handle},
        {ECellType::Gum, get_sprite_handle("gum")},
        {ECellType::BiGum, get_sprite_handle("bigum")},
        {ECellType::Door, get_sprite_handle("door")}
    };

    std::unordered_map<EItemType, SpriteHandle> map_item_type = {
        {EItemType::Cherry, get_sprite_handle("cherry")},
        {EItemType::Strawberry, get_sprite_handle("strawberry")},
        {EItemType::Abricot, get_sprite_handle("abricot")},
        {EItemType::Apple, get_sprite_handle("apple")},
        {EItemType::Wtfruit, get_sprite_handle("wtfruit")},
        {EItemType::Axe, get_sprite_handle("axe")},
        {EItemType::Bell, get_sprite_handle("bell")},
        {EItemType::Key, get_sprite_handle("key")},
    };

    std::array<SpriteHandle, 16> walls;
    walls[0]                                                  = get_sprite_handle("wall_none");
    walls[WALL_MASK_NORTH]                                    = get_sprite_handle("wall_N");
    walls[WALL_MASK_EAST]                                     = get_sprite_handle("wall_E");
    walls[WALL_MASK_EAST | WALL_MASK_NORTH]                   = get_sprite_handle("wall_NE");
    walls[WALL_MASK_WEST]                                     = get_sprite_handle("wall_W");
    walls[WALL_MASK_WEST | WALL_MASK_NORTH]                   = get_sprite_handle("wall_NW");
    walls[WALL_MASK_WEST | WALL_MASK_EAST]                    = get_sprite_handle("wall_EW");
    walls[WALL_MASK_WEST | WALL_MASK_EAST | WALL_MASK_NORTH]  = get_sprite_handle("wall_NEW");
    walls[WALL_MASK_SOUTH]                                    = get_sprite_handle("wall_S");
    walls[WALL_MASK_SOUTH | WALL_MASK_NORTH]                  = get_sprite_handle("wall_NS");
    walls[WALL_MASK_SOUTH | WALL_MASK_EAST]                   = get_sprite_handle("wall_ES");
    walls[WALL_MASK_SOUTH | WALL_MASK_EAST | WALL_MASK_NORTH] = get_sprite_handle("wall_NES");
    walls[WALL_MASK_SOUTH | WALL_MASK_WEST]                   = get_sprite_handle("wall_WS");
    walls[WALL_MASK_SOUTH | WALL_MASK_WEST | WALL_MASK_NORTH] = get_sprite_handle("wall_NWS");
    walls[WALL_MASK_SOUTH | WALL_MASK_WEST | WALL_MASK_EAST]  = get_sprite_handle("wall_EWS");
    walls[WALL_MASK_FULL]                                     = get_sprite_handle("wall_full");

    for (auto& cell : grid)
        cell.update_sprite_handle(map_cell_type, map_item_type, walls);
}

void Terrain::create_wall_cache_surface()
{
    if (wall_cache_surface_handle != nullptr)
        free_wall_cache_surface();

    SDL_Surface* esh = pm::Engine::get().get_surface_handle();

    wall_cache_surface_handle = SDL_CreateRGBSurface(0, esh->w, esh->h, 32, 0, 0, 0, 0);

    for (auto& cell : grid)
        if (cell.get_type() == ECellType::Wall)
            cell.draw(wall_cache_surface_handle);

    if (wall_cache_surface_handle == nullptr)
        INFO("couldn't create wall cache surface, continuing without wall caching");
}

void Terrain::free_wall_cache_surface()
{
    SDL_FreeSurface(wall_cache_surface_handle);
    wall_cache_surface_handle = nullptr;
}

Cell& Terrain::get_cell(const uint32_t x, const uint32_t y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        FATAL("Cannot read grid cell {}/{}", x, y);

    return grid[x + y * width];
}

void Terrain::draw()
{
    if (wall_cache_surface_handle != nullptr)
        SDL_BlitSurface(wall_cache_surface_handle, nullptr, pm::Engine::get().get_surface_handle(), nullptr);

    for (auto& cell : grid)
        if (wall_cache_surface_handle == nullptr || cell.get_type() != ECellType::Wall)
            cell.draw();
}

Terrain::~Terrain()
{
    free_wall_cache_surface();
}
}
