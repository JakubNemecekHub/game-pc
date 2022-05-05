#pragma once

#include <iostream>

#include <SDL2/SDL.h>

class ControlManager
{
private:
    static ControlManager* singleton_;
    ControlManager() {};
public:
    static ControlManager* GetInstance();
    void startUp();
    void shutDown();
    SDL_Event event;
    void handleEvents();
};
