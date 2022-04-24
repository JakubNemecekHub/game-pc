#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <iostream>
#include <string>
#include <array>
#include <queue>

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
    ~RenderManager() {};    // Descturctor that does nothing
    void startUp();         // Method that I can use instead of contructor
    void shutDown();        // Method that I can use instead of desctructor

    // Load Stuff.

    static Texture* load_texture(std::string file_name);
    static SDL_Texture* load_sdl_texture(std::string file_name);
    static SDL_Surface* load_bitmap(std::string file_name);
    static SDL_Texture* texture_from_surface(SDL_Surface* surface);

    // Render stuff.

    void register_object(RenderableObject* r);
    void render();

    // Helper functions.

    void scale_full_h(Texture* &texture);
    int get_screen_width();
};

#endif // RENDER_MANAGER_H