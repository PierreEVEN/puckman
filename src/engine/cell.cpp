#include "cell.hpp"

#include "logger.hpp"

namespace pm
{
Cell::Cell(const char chr)
{
    switch (chr)
    {
    case '.':
        type = ECellType::Void;
        break;
    case '#':
        type = ECellType::Wall;
        break;
    case 'o':
        type = ECellType::Gum;
        break;
    case 'G':
        type = ECellType::BiGum;
        break;
    case '-':
        type = ECellType::Door;
        break;
    default: 
        type = ECellType::Item;

        switch (chr)
        {
        default:
            ERROR("Unhandled item type : {}", chr);
        }
        break;
    }
}

Cell::Cell(const ECellType in_type, const EItemType in_item_type)
    : type(in_type), item_type(in_item_type)
{
}
}
