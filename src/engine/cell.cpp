#include "cell.hpp"

#include "logger.hpp"

namespace pm
{
Cell::Cell() : type(ECellType::Void)
{}

void Cell::set_item(EItemType in_item_type)
{
    type      = ECellType::Item;
    item_type = in_item_type;
}

void Cell::set_wall(EWallType in_wall_type)

{
    type      = ECellType::Wall;
    wall_type = in_wall_type;
}

void Cell::set_gum(bool big)
{
    type = big ? ECellType::Gum : ECellType::BiGum;
}

void Cell::set_door()
{
    type = ECellType::Door;
}

void Cell::draw()
{
}

Cell Cell::from_char(char chr)
{
    Cell cell;
    switch (chr)
    {
    case '.':
        return cell;
    case '#':
        cell.set_wall(EWallType::UBLR);
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
