#include "pacman.hpp"
#include "engine/sprite_sheet.hpp"

namespace pm
{
void Pacman::load_sprites()
{
    // Load terrain sprites
    sprite_sheet->new_sprite("gum", {128, 32, 16, 16}, 10, {});
    sprite_sheet->new_sprite("bigum", {144, 32, 16, 16}, 10, {});
    sprite_sheet->new_sprite("door", {128, 96, 16, 16}, 10, {});
    sprite_sheet->new_sprite("cherry", {128, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet->new_sprite("strawberry", {144, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet->new_sprite("abricot", {160, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet->new_sprite("apple", {176, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet->new_sprite("wtfruit", {192, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet->new_sprite("axe", {208, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet->new_sprite("bell", {224, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet->new_sprite("key", {240, 64, 16, 16}, 10, {{0, -16}});
    sprite_sheet->new_sprite("wall_none", {184, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_N", {176, 88, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_E", {176, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_NE", {168, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_W", {168, 88, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_NW", {160, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_EW", {128, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_NEW", {144, 88, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_S", {184, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_NS", {136, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_ES", {144, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_NES", {152, 88, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_WS", {152, 80, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_NWS", {136, 88, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_EWS", {128, 88, 8, 8}, 10, {});
    sprite_sheet->new_sprite("wall_full", {160, 88, 8, 8}, 10, {});

    sprite_sheet->new_sprite("pacman_default", {0, 0, 16, 16}, 20, {{16, 0}, {32, 0}, {16, 0}});
    sprite_sheet->new_sprite("pacman_right", {0, 0, 16, 16}, 20, {{16, 0}, {32, 0}, {16, 0}});
    sprite_sheet->new_sprite("pacman_left", {0, 0, 16, 16}, 20, {{48, 0}, {64, 0}, {48, 0}});
    sprite_sheet->new_sprite("pacman_down", {0, 0, 16, 16}, 20, {{112, 0}, {128, 0}, {112, 0}});
    sprite_sheet->new_sprite("pacman_up", {0, 0, 16, 16}, 20, {{80, 0}, {96, 0}, {80, 0}});

    sprite_sheet->new_sprite("ghost_a_default", {0, 32, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_a_right", {0, 32, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_a_left", {32, 32, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_a_down", {96, 32, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_a_up", {64, 32, 16, 16}, 10, {{16, 0}});

    sprite_sheet->new_sprite("ghost_b_default", {0, 48, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_b_right", {0, 48, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_b_left", {32, 48, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_b_down", {96, 48, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_b_up", {64, 48, 16, 16}, 10, {{16, 0}});

    sprite_sheet->new_sprite("ghost_c_default", {0, 64, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_c_right", {0, 64, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_c_left", {32, 64, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_c_down", {96, 64, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_c_up", {64, 64, 16, 16}, 10, {{16, 0}});

    sprite_sheet->new_sprite("ghost_d_default", {0, 80, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_d_right", {0, 80, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_d_left", {32, 80, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_d_down", {96, 80, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("ghost_d_up", {64, 80, 16, 16}, 10, {{16, 0}});

    sprite_sheet->new_sprite("pac²man_die",
                             {0, 0, 16, 16},
                             20,
                             {{80, 0}, {96, 0}, {0, 16}, {16, 16}, {32, 16}, {48, 16}, {64, 16}, {80, 16}, {96, 16}, {112, 16}, {128, 16}, {144, 16}});

    sprite_sheet->new_sprite("frightened_ghost", {0, 95, 16, 16}, 10, {{16, 0}});
    sprite_sheet->new_sprite("frightened_ghost_flash", {0, 95, 16, 16}, 10, {{16, 0}, {32, 0}, {48, 0}});
    
    sprite_sheet->new_sprite("pacman_life", {48, 0, 16, 16}, 20, {});
    sprite_sheet->new_sprite("eyes_down", {112, 96, 16, 16}, 20, {});
    sprite_sheet->new_sprite("eyes_left", {80, 96, 16, 16}, 20, {});
    sprite_sheet->new_sprite("eyes_right", {64, 96, 16, 16}, 20, {});
    sprite_sheet->new_sprite("eyes_up", {96, 96, 16, 16}, 20, {});
}
}
