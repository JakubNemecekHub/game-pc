#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <iostream>
#include <string>
#include <stack>            // Stack to store Textures to render
#include <unordered_map>    // Map of animations for a sprite

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../components/Texture.hpp"    // For Simple Texture
#include "../components/Sprite.hpp"     // For Animation


class RenderManager
{
private:
    // Fields
    // Singleton
    static RenderManager* singleton_;
    // Normal fields
    SDL_Window* window;
    std::stack<Texture*> texture_stack;

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
    static Texture* loadTexture(const char* fileName);                                      // Loads Single texture
    static std::unordered_map<std::string, Animation> loadSprite(const char* file_name);    // Loads Spritesheet from a file
    void registerTexture(Texture* texture);
    void render();
};

#endif // RENDER_MANAGER_H