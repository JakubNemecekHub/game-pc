#include <iostream>

#include <SDL2/SDL.h>

#include "Game.hpp"


uint32_t tick {SDL_GetTicks()};


int main(int argc, char* args[])
{
    Game game;

    // Game loop
    while ( game.running() )
    {
        uint32_t next_tick { SDL_GetTicks() };
        int dt = next_tick - tick;
        tick = next_tick;
        game.update(dt);
    }
    return 0;
}