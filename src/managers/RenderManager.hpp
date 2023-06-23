#pragma once

#include <iostream>
#include <string>
#include <array>
#include <queue>
#include <tuple>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "LogManager.hpp"
#include "../math/Polygon.hpp"
#include "../math/Vector2D.hpp"


class Sprite;

namespace MyType
{

/*
    Type erasure for Polygon and Vector2D, and also Sprites.
    According to:
        https://www.modernescpp.com/index.php/type-erasure
        https://davekilian.com/cpp-type-erasure.html
*/
class Object
{
public:
 
    // Define the abstract class (or interface) of the objects this wrapper can hold
    struct Concept
    {
        virtual ~Concept() {}
        // Pure virtual functions
        virtual void render(SDL_Renderer* renderer) const = 0;
    };
    
    // Methods delegating call to the object itself
    void render(SDL_Renderer* renderer) const { object->render(renderer); }

    // What is this?
    template<typename T>
    struct Model : Concept
    {
        Model(T* t) // cont T& t
            : object{t} {}
        void render(SDL_Renderer* renderer) const override { object->render(renderer); }
    private:
        T* object;
    };
    
    std::shared_ptr<Concept> object; // This holds the object itself

    // Templated constructor
    template<typename T>
    Object(T* obj)
        : object{std::make_shared<Model<T>>(obj)} {}
};

} // namespace



class RenderManager
{
private:

    LogManager*             log_;
    SDL_Window*             window_;
    static SDL_Renderer*    renderer_;
    static const int        MAX_LAYERS_ {5};
    std::array<std::queue<MyType::Object>, MAX_LAYERS_> render_queues_;

    void render_sprite();

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
    bool submit(Polygon* polygon);
    bool submit(Vector2D* vector2d);
    void render();

    // Helper functions.

    void scale_full_h(Sprite* sprite);
    void center_horizontally(Sprite* sprite);
    void center_vertically(Sprite* sprite);
    int get_screen_width();
    
};
