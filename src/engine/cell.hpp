#pragma once
#include "sprite_sheet.hpp"

#include <memory>

namespace pm {

enum class EItemType
{
    Cherry,
    Strawberry,
    Abricot,
    Apple,
    Wtfruit,
    Axe,
    Bell,
    Key
};

enum class EWallType
{
    LR,
    UB,
    BR,
    LB,
    LU,
    UR,
    LRB,
    ULB,
    LRU,
    UBR,
    UBLR,
    L,
    U,
    B,
    R
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

    static Cell from_char(char chr);

    Cell();
    
    void set_item(EItemType in_item_type);
    void set_wall(EWallType in_wall_type);
    void set_gum(bool big);
    void set_door();

    void draw();

private:

    std::unique_ptr<SpriteHandle> sprite_handle = nullptr;

    ECellType type = ECellType::Void;

    union
    {
        EItemType item_type;
        EWallType wall_type;
    };
};
}
