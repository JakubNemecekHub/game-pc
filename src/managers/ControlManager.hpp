#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

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


#endif // CONTROL_MANAGER_H