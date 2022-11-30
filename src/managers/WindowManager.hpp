#pragma once

#include <string>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>

#include "LogManager.hpp"


class WindowManager
{
private:

    LogManager* log_;
    SDL_Window* window_;
    std::string title_;
    int         width_;
    int         height_;
    bool        fullscreen_initial_;

public:

    bool running;

    WindowManager() {};
    WindowManager(LogManager* log, YAML::Node ini);

    bool startUp();
    bool shutDown();

    SDL_Window* window();
    void toggle_fullscreen();
    void close();
};
