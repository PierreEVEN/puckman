#pragma once

namespace pm {

enum class EItemType
{
    Cherry,
    ///...
};

enum class ECellType
{
    Void,
    Wall,
    Gum,
    Item,
    BiGum,
    Door,
};

class Cell
{
public:
    Cell(const char chr);
    Cell(const ECellType in_type, const EItemType in_item_type);
private:
    ECellType type;
    EItemType item_type;
};

}
