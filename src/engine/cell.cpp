#include "cell.hpp"

#include "logger.hpp"

namespace pm
{

double Cell::draw_scale = 2.;

Cell::Cell() : type(ECellType::Void)
{}

void Cell::set_pos(const SDL_Point& in_pos)
{
    pos = in_pos;
}

void Cell::set_item(EItemType in_item_type)
{
    type      = ECellType::Item;
    item_type = in_item_type;
}

void Cell::set_wall(WallMask in_wall_mask)
{
    type      = ECellType::Wall;
    wall_mask = in_wall_mask;
}

void Cell::set_gum(bool big)
{
    type = big ? ECellType::Gum : ECellType::BiGum;
}

void Cell::set_door()
{
    type = ECellType::Door;
}

ECellType Cell::get_type() const
{
    return type;
}

void Cell::update_sprite_handle(
        std::unordered_map<ECellType, SpriteHandle> map_cell_type,
        std::unordered_map<EItemType, SpriteHandle> map_item_type,
        std::array<SpriteHandle, 16>& walls)
{
    switch (type)
    {
    case ECellType::Wall:
        sprite_handle = walls[wall_mask];
        break;
    case ECellType::Item:
        sprite_handle = map_item_type[item_type];
        break;
    default:
        sprite_handle = map_cell_type[type];
        break;
    }
}

void Cell::draw()
{
    if (sprite_handle){
        SDL_Point draw_pos{pos};
        const int size = 8 * (1 + (type != ECellType::Wall));
        draw_pos.x = static_cast<int>(8 * draw_pos.x * draw_scale);
        draw_pos.y = static_cast<int>(8 * draw_pos.y * draw_scale);
        sprite_handle.draw(draw_pos, draw_scale, draw_scale);
    }
}

Cell Cell::from_char(char chr)
{
    Cell cell;
    switch (chr)
    {
    case '.':
        return cell;
    case '#':
        cell.set_wall(WALL_MASK_FULL);
        break;
    case 'o':
        cell.set_gum(false);
        break;
    case 'G':
        cell.set_gum(true);
        break;
    case '-':
        cell.set_door();
        break;
    default:
        cell.set_item(EItemType::Abricot);
    }
    return cell;
}
}
