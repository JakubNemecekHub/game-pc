#pragma once

#include <iostream>
#include <string>
#include <array>
#include <queue>
// #include <memory>   // unique_ptr

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LogManager.hpp"
#include "../components/RenderableObject.h"
#include "../components/Texture.hpp"

class Texture;


class RenderManager
{
private:

    LogManager*             log_;
    SDL_Window*             window_;
    static SDL_Renderer*    renderer_;
    static const int        MAX_LAYERS_ {4};
    std::array<std::queue<RenderableObject*>, MAX_LAYERS_> render_objects_;

public:

    RenderManager() {};
    RenderManager(LogManager* log);

    bool startUp(SDL_Window* window);
    bool shutDown();

    // Load Stuff.

    SDL_Texture* load_sdl_texture(std::string file_name);
    SDL_Surface* load_bitmap(std::string file_name);
    SDL_Texture* texture_from_surface(SDL_Surface* surface);

    // Render stuff.

    bool register_object(RenderableObject* r);
    void render();

    // Helper functions.

    void scale_full_h(Texture* texture);
    int get_screen_width();
};
