#include "terrain.hpp"
#include "engine.hpp"
#include "logger.hpp"
#include "game/pacman_gamemode.hpp"

#include <fstream>
#include <SDL_surface.h>

namespace pm
{
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

    initial_grid.reserve(static_cast<size_t>(width) * static_cast<size_t>(height));
    initial_gum_count = 0;
    for (const auto& s_line : lines)
        for (const auto& cell : s_line)
        {
            initial_grid.emplace_back(Cell::from_char(cell));
            if (initial_grid.back().get_type() == ECellType::Gum)
                initial_gum_count++;
        }

    INFO("Successfully loaded {}", path.string());
}

void Terrain::eat(const Vector2I& pos)
{
    if (pos.x() < 0 || pos.y() < 0 || static_cast<uint32_t>(pos.x()) >= width || static_cast<uint32_t>(pos.y()) >= height)
        return;

    auto&     cell          = get_cell(pos);
    int32_t   points        = 0;
    EItemType spawned_fruit = EItemType::Cherry;

    switch (cell.get_type())
    {
    case ECellType::Gum:
        cell.update_type(ECellType::Void);
        gum_count--;
        if (gum_count == 70 || gum_count == 170)
        {
            const auto level = Engine::get().get_gamemode<PacmanGamemode>().current_level();
            switch (level)
            {
            case 1:
                spawned_fruit = EItemType::Cherry;
                break;
            case 2:
                spawned_fruit = EItemType::Strawberry;
                break;
            case 3:
            case 4:
                spawned_fruit = EItemType::Abricot;
                break;
            case 5:
            case 6:
                spawned_fruit = EItemType::Apple;
                break;
            case 7:
            case 8:
                spawned_fruit = EItemType::Grapes;
                break;
            case 9:
            case 10:
                spawned_fruit = EItemType::Galaxian;
                break;
            case 11:
            case 12:
                spawned_fruit = EItemType::Bell;
                break;
            default: ;
                spawned_fruit = EItemType::Key;
            }

            item_timer = Engine::get().random_double(9.3333, 10);
            get_cell({10, 15}).set_item(spawned_fruit);
        }
        update_sprite_handles();
        break;
    case ECellType::Item:
        switch (cell.get_item())
        {
        case EItemType::Cherry:
            points = 100;
            break;
        case EItemType::Strawberry:
            points = 300;
            break;
        case EItemType::Abricot:
            points = 500;
            break;
        case EItemType::Apple:
            points = 700;
            break;
        case EItemType::Grapes:
            points = 1000;
            break;
        case EItemType::Galaxian:
            points = 2000;
            break;
        case EItemType::Bell:
            points = 3000;
            break;
        case EItemType::Key:
            points = 5000;
            break;
        default:
            points = 0;
        }
        Engine::get().get_gamemode<PacmanGamemode>().add_points(points);
        cell.update_type(ECellType::Void);
        update_sprite_handles();
        break;
    case ECellType::BiGum:
        Engine::get().get_gamemode<PacmanGamemode>().on_frightened.execute();
        cell.update_type(ECellType::Void);
        update_sprite_handles();
        break;
    default:
        break;
    }
}

Vector2I Terrain::closest_free_point(const Vector2I& location) const
{
    const Vector2I clamped_location = {std::clamp(location.x(), 0, static_cast<int>(width)), std::clamp(location.y(), 0, static_cast<int>(height))};

    for (int i = 0; i < static_cast<int>(std::max(width, height)); ++i)
    {
        for (int x = -i; x <= i; ++x)
            if (is_free(clamped_location + Vector2I{i, x}))
                return clamped_location + Vector2I{i, x};

        for (int x = -i; x <= i; ++x)
            if (is_free(clamped_location + Vector2I{-i, x}))
                return clamped_location + Vector2I{-i, x};

        for (int y = -i + 1; y < i; ++y)
            if (is_free(clamped_location + Vector2I{y, i}))
                return clamped_location + Vector2I{y, y};

        for (int y = -i + 1; y < i; ++y)
            if (is_free(clamped_location + Vector2I{y, -i}))
                return clamped_location + Vector2I{y, -i};
    }
    FATAL("failed to find free point");
}

void Terrain::set_wall_color(const Uint8 r, const Uint8 g, const Uint8 b) const
{
    SDL_SetSurfaceColorMod(wall_cache_surface_handle, r, g, b);
}

void Terrain::reset()
{
    gum_count = initial_gum_count;
    grid      = initial_grid;
    update_position_and_walls();

    update_sprite_handles();

    create_wall_cache_surface();
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
                Cell::WallMask mask = 0;
                if (y > 0 && grid[(y - 1) * width + x].get_type() == ECellType::Wall)
                    mask |= Cell::WALL_MASK_NORTH;
                if (x > 0 && grid[y * width + (x - 1)].get_type() == ECellType::Wall)
                    mask |= Cell::WALL_MASK_WEST;
                if (x + 1 < width && grid[y * width + (x + 1)].get_type() == ECellType::Wall)
                    mask |= Cell::WALL_MASK_EAST;
                if (y + 1 < height && grid[(y + 1) * width + x].get_type() == ECellType::Wall)
                    mask |= Cell::WALL_MASK_SOUTH;
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

    const std::unordered_map<ECellType, SpriteHandle> map_cell_type = {
        {ECellType::Void, null_handle},
        {ECellType::Gum, get_sprite_handle("gum")},
        {ECellType::BiGum, get_sprite_handle("bigum")},
        {ECellType::Door, get_sprite_handle("door")}
    };

    const std::unordered_map<EItemType, SpriteHandle> map_item_type = {
        {EItemType::Cherry, get_sprite_handle("cherry")},
        {EItemType::Strawberry, get_sprite_handle("strawberry")},
        {EItemType::Abricot, get_sprite_handle("abricot")},
        {EItemType::Apple, get_sprite_handle("apple")},
        {EItemType::Grapes, get_sprite_handle("wtfruit")},
        {EItemType::Galaxian, get_sprite_handle("axe")},
        {EItemType::Bell, get_sprite_handle("bell")},
        {EItemType::Key, get_sprite_handle("key")},
    };

    std::array<SpriteHandle, 16> walls;
    walls[0]                                                                    = get_sprite_handle("wall_none");
    walls[Cell::WALL_MASK_NORTH]                                                = get_sprite_handle("wall_N");
    walls[Cell::WALL_MASK_EAST]                                                 = get_sprite_handle("wall_E");
    walls[Cell::WALL_MASK_EAST | Cell::WALL_MASK_NORTH]                         = get_sprite_handle("wall_NE");
    walls[Cell::WALL_MASK_WEST]                                                 = get_sprite_handle("wall_W");
    walls[Cell::WALL_MASK_WEST | Cell::WALL_MASK_NORTH]                         = get_sprite_handle("wall_NW");
    walls[Cell::WALL_MASK_WEST | Cell::WALL_MASK_EAST]                          = get_sprite_handle("wall_EW");
    walls[Cell::WALL_MASK_WEST | Cell::WALL_MASK_EAST | Cell::WALL_MASK_NORTH]  = get_sprite_handle("wall_NEW");
    walls[Cell::WALL_MASK_SOUTH]                                                = get_sprite_handle("wall_S");
    walls[Cell::WALL_MASK_SOUTH | Cell::WALL_MASK_NORTH]                        = get_sprite_handle("wall_NS");
    walls[Cell::WALL_MASK_SOUTH | Cell::WALL_MASK_EAST]                         = get_sprite_handle("wall_ES");
    walls[Cell::WALL_MASK_SOUTH | Cell::WALL_MASK_EAST | Cell::WALL_MASK_NORTH] = get_sprite_handle("wall_NES");
    walls[Cell::WALL_MASK_SOUTH | Cell::WALL_MASK_WEST]                         = get_sprite_handle("wall_WS");
    walls[Cell::WALL_MASK_SOUTH | Cell::WALL_MASK_WEST | Cell::WALL_MASK_NORTH] = get_sprite_handle("wall_NWS");
    walls[Cell::WALL_MASK_SOUTH | Cell::WALL_MASK_WEST | Cell::WALL_MASK_EAST]  = get_sprite_handle("wall_EWS");
    walls[Cell::WALL_MASK_FULL]                                                 = get_sprite_handle("wall_full");

    for (auto& cell : grid)
        cell.update_sprite_handle(map_cell_type, map_item_type, walls);
}

void Terrain::create_wall_cache_surface()
{
    if (wall_cache_surface_handle != nullptr)
        free_wall_cache_surface();

    SDL_Surface* esh = pm::Engine::get().get_surface_handle();

    wall_cache_surface_handle = SDL_CreateRGBSurface(0, esh->w, esh->h, 32, 0, 0, 0, 0);

    if (wall_cache_surface_handle == nullptr)
    {
        INFO("couldn't create wall cache surface, continuing without wall caching");
        return;
    }

    for (auto& cell : grid)
        if (cell.get_type() == ECellType::Wall)
            cell.draw(unit_length, wall_cache_surface_handle);
}

void Terrain::free_wall_cache_surface()
{
    SDL_FreeSurface(wall_cache_surface_handle);
    wall_cache_surface_handle = nullptr;
}

Cell& Terrain::get_cell(const Vector2I& pos)
{
    if (pos.x() < 0 || pos.x() >= static_cast<int32_t>(width) || pos.y() < 0 || pos.y() >= static_cast<int32_t>(height))
        FATAL("Cannot read grid cell {}/{}", pos.x(), pos.y());

    return grid[pos.x() + pos.y() * width];
}

void Terrain::tick(double delta_time)
{
    if (item_timer > 0)
    {
        item_timer -= delta_time;
        if (item_timer <= 0)
            get_cell({10, 15}).update_type(ECellType::Void);

    }
}

void Terrain::draw()
{
    if (wall_cache_surface_handle != nullptr)
        SDL_BlitSurface(wall_cache_surface_handle, nullptr, pm::Engine::get().get_surface_handle(), nullptr);

    for (auto& cell : grid)
        if (wall_cache_surface_handle == nullptr || cell.get_type() != ECellType::Wall)
            cell.draw(unit_length);
}

Terrain::~Terrain()
{
    free_wall_cache_surface();
}
}
