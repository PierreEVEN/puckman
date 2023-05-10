#include "engine.hpp"

#include <iostream>
#include <SDL.h>

#include "logger.hpp"

#include <random>

namespace pm
{
static Engine* ENGINE_SINGLETON = nullptr;

bool Engine::next_frame()
{
    gamemode->tick(get_delta_second());
    gamemode->draw();

    SDL_UpdateWindowSurface(window_handle);

    delta_second = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - last_frame_time).count()) / 1000000.0;
    //clamp delta seconds to avoid physic issues during freezes. (minimum framerate is 60fps)
    delta_second    = std::min(1 / 60.0, delta_second);
    last_frame_time = std::chrono::steady_clock::now();
    SDL_FillRect(surface_handle, nullptr, 0x000000);
    return !close;
}

void Engine::shutdown()
{
    close = true;
    INFO("Request shutdown");
}

int32_t Engine::random_int(int32_t min, int32_t max)
{
    return std::uniform_int_distribution(min, max)(mt);
}

double Engine::random_double(double min, double max)
{
    return std::uniform_real_distribution(min, max)(mt);
}

Engine::Engine(const std::string& app_name, uint32_t width, uint32_t height)
    : window_handle(nullptr), surface_handle(nullptr), close(false), delta_second(0.033), last_frame_time(std::chrono::steady_clock::now()), start_time(std::chrono::steady_clock::now()), mt(random_device())
{
    INFO("Initializing SDL");

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        FATAL("Failed to initialize SDL ({})", SDL_GetError());
    }

    window_handle  = SDL_CreateWindow(app_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    surface_handle = SDL_GetWindowSurface(window_handle);
}


Engine::~Engine()
{
    SDL_Quit();
    ENGINE_SINGLETON = nullptr;
    INFO("Engine closed");
}

Engine& Engine::init_internal(const std::string& app_name, uint32_t width, uint32_t height)
{
    if (ENGINE_SINGLETON)
        FATAL("cannot create multiple engine instances");
    ENGINE_SINGLETON = new Engine(app_name, width, height);
    return *ENGINE_SINGLETON;
}

Engine& Engine::get()
{
    if (!ENGINE_SINGLETON)
        FATAL("pc::Engine::init() should have been called before");
    return *ENGINE_SINGLETON;
}
}
