#include "engine.hpp"

#include <iostream>
#include <SDL.h>

#include "logger.hpp"

namespace pc
{
static std::unique_ptr<Engine> ENGINE_SINGLETON = nullptr;

bool Engine::next_frame()
{
    SDL_UpdateWindowSurface(window_handle);

    delta_second    = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - last_frame_time).count()) / 1000000.0;
    last_frame_time = std::chrono::steady_clock::now();

    return !close;
}

void Engine::shutdown()
{
    close = true;
}

Engine::Engine(const std::string& app_name, uint32_t width, uint32_t height)
    : window_handle(nullptr), surface_handle(nullptr), close(false), delta_second(0.033), last_frame_time(std::chrono::steady_clock::now()), start_time(std::chrono::steady_clock::now())
{
    INFO("Initializing SDL");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        char error_msg[512];
        FATAL("Failed to initialize SDL (%d) : %s", SDL_GetError(), SDL_GetErrorMsg(error_msg, sizeof(error_msg)));
    }

    window_handle  = SDL_CreateWindow(app_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    surface_handle = SDL_GetWindowSurface(window_handle);
}


Engine::~Engine()
{
    SDL_Quit();
    ENGINE_SINGLETON = nullptr;
}

Engine& Engine::init(const std::string& app_name, uint32_t width, uint32_t height)
{
    if (ENGINE_SINGLETON)
        FATAL("cannot create multiple engine instances");
    ENGINE_SINGLETON = std::unique_ptr<Engine>(new Engine(app_name, width, height));
    return *ENGINE_SINGLETON;
}

Engine& Engine::get()
{
    if (!ENGINE_SINGLETON)
        FATAL("pc::Engine::init() should have been called before");
    return *ENGINE_SINGLETON;
}
}
