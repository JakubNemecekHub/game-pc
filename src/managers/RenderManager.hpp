#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <iostream>
#include <string>
#include <stack>            // Stack to store Textures to render
#include <unordered_map>    // Map of animations for a sprite

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../components/Texture.hpp"    // For Simple Texture
// #include "../components/Sprite.hpp"     // For Animation


class RenderManager
{
private:
    // Fields
    // Singleton
    static RenderManager* singleton_;
    // Normal fields
    SDL_Window* window;
    std::stack<Texture*> room_background;
    std::stack<Texture*> room_ambient;
    // Methods
    // Constructor
    RenderManager() {};
public:
    // Fields
    static SDL_Renderer* renderer;

    // Methods
    static RenderManager* GetInstance();    // Singleton logic
    ~RenderManager() {};    // Descturctor that does nothing
    void startUp();         // Method that I can use instead of contructor
    void shutDown();        // Method that I can use instead of desctructor
    // Load Stuff.
    static Texture* load_texture(std::string fileName);         // Loads Single texture
    static SDL_Texture* load_sdl_texture(std::string fileName); // Loads Single texture
    // Render stuff.
    void register_room_texture(Texture* texture);
    void register_ambient_texture(Texture* texture);
    void render();
    // Helper functions.
    void scale_full_h(Texture* &texture);
    int get_screen_width();
};

#endif // RENDER_MANAGER_H