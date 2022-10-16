#pragma once

#include <iostream>
#include <string>
#include <array>
#include <queue>
#include <memory>   // unique_ptr

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../components/RenderableObject.h"
#include "../components/Texture.hpp"


class RenderManager
{
private:
    // Fields
    // Singleton
    static RenderManager* singleton_;
    // Normal fields
    SDL_Window* window;

    static const int MAX_LAYERS {4};
    std::array<std::queue<RenderableObject*>, MAX_LAYERS> render_objects;

    // Methods
    // Constructor
    RenderManager() {};
public:
    // Fields
    static SDL_Renderer* renderer;  // I would like to move this to be private

    // Methods
    static RenderManager* GetInstance();    // Singleton logic
    ~RenderManager() {};    // Destructor that does nothing
    void startUp();         // Method that I can use instead of constructor
    void shutDown();        // Method that I can use instead of destructor

    // Load Stuff.

    static Texture load_texture(std::string file_name);
    static SDL_Texture* load_sdl_texture(std::string file_name);
    static SDL_Surface* load_bitmap(std::string file_name);
    static SDL_Texture* texture_from_surface(SDL_Surface* surface);

    // Render stuff.

    void register_object(RenderableObject* r);
    void render();

    // Helper functions.

    void scale_full_h(Texture& texture);
    void scale_full_h(const std::unique_ptr<Texture>& texture); // Can use templates?
    int get_screen_width();
};
