#pragma once

#include <iostream>
#include <string>
#include <array>
#include <queue>
#include <tuple>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LogManager.hpp"
#include "../components/Sprite.hpp"
#include "../math/Polygon.hpp"
#include "../math/Vector2D.hpp"

class Sprite;


class RenderManager
{
private:

    LogManager*             log_;
    SDL_Window*             window_;
    static SDL_Renderer*    renderer_;
    static const int        MAX_LAYERS_ {4};
    std::array<std::queue<Sprite*>, MAX_LAYERS_>    render_queues_;
    std::queue<std::tuple<SDL_Surface*, SDL_Rect*>> surface_queue_;
    std::queue<Polygon*>                            polygon_queue_;
    std::queue<Vector2D*>                           vector_queue_;

    void render_surface();
    void render_math();

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

    bool submit(Sprite* sprite);
    bool submit(SDL_Surface* surface, SDL_Rect* dest_rect);
    bool submit(Polygon* polygon);
    bool submit(Vector2D* vector2d);
    void render();

    // Helper functions.

    void scale_full_h(Sprite* sprite);
    void center_horizontally(Sprite* sprite);
    int get_screen_width();
};
