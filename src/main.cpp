#include <SDL.h>

#include <iostream>
#include <filesystem>

#include "engine.hpp"
#include "logger.hpp"

SDL_Surface* plancheSprites = nullptr;

SDL_Rect src_bg = {200, 3, 168, 216}; // x,y, w,h (0,0) en haut a gauche
SDL_Rect bg     = {4, 4, 672, 864};   // ici scale x4

SDL_Rect ghost_r = {3, 123, 16, 16};
SDL_Rect ghost_l = {37, 123, 16, 16};
SDL_Rect ghost_d = {105, 123, 16, 16};
SDL_Rect ghost_u = {71, 123, 16, 16};
SDL_Rect ghost   = {34, 34, 32, 32}; // ici scale x2

int count;


// fonction qui met à jour la surface de la fenetre "win_surf"
void draw()
{
    SDL_SetColorKey(plancheSprites, false, 0);
    SDL_BlitScaled(plancheSprites, &src_bg, pc::Engine::get().get_surface_handle(), &bg);

    // petit truc pour faire tourner le fantome
    SDL_Rect* ghost_in = nullptr;
    switch (count / 128)
    {
    case 0:
        ghost_in = &(ghost_r);
        ghost.x++;
        break;
    case 1:
        ghost_in = &(ghost_d);
        ghost.y++;
        break;
    case 2:
        ghost_in = &(ghost_l);
        ghost.x--;
        break;
    case 3:
        ghost_in = &(ghost_u);
        ghost.y--;
        break;
    }
    count = (count + 1) % (512);

    // ici on change entre les 2 sprites sources pour une jolie animation.
    SDL_Rect ghost_in2 = *ghost_in;
    if ((count / 4) % 2)
        ghost_in2.x += 17;

    // couleur transparente
    SDL_SetColorKey(plancheSprites, true, 0);
    // copie du sprite zoomé
    SDL_BlitScaled(plancheSprites, &ghost_in2, pc::Engine::get().get_surface_handle(), &ghost);
}


int main(int argc, char** argv)
{
    auto engine = pc::Engine::init("PucMan", 700, 900);

    if (!std::filesystem::exists("./resources/pacman_sprites.bmp"))
        FATAL("sprite not found");
    plancheSprites = SDL_LoadBMP("./resources/pacman_sprites.bmp");
    count          = 0;

    // BOUCLE PRINCIPALE
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (!quit && SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                break;
            }
        }

        // Gestion du clavier        
        int          nbk;
        const Uint8* keys = SDL_GetKeyboardState(&nbk);
        if (keys[SDL_SCANCODE_ESCAPE])
            quit = true;
        if (keys[SDL_SCANCODE_LEFT])
            puts("LEFT");
        if (keys[SDL_SCANCODE_RIGHT])
            puts("RIGHT");

        // AFFICHAGE
        draw();

        pc::Engine::get().next_frame();
    }
    return 0;
}
