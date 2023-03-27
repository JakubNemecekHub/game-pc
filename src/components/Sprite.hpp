#pragma once

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "../managers/RenderManager.hpp"
#include "../math/Vector2D.hpp"

#define STATIC_ID "static"
class RenderManager;

class Frame
{
public:

    SDL_Rect src_rect_;
    int      duration_;
    Frame()
        : src_rect_{0, 0, 0, 0}, duration_{0} {};
    Frame(int x, int y, int w, int h, int duration)
        : src_rect_{x, y, w, h}, duration_{duration} {};
    Frame(std::vector<int> frame)
        : Frame(frame.at(0), frame.at(1), frame.at(2), frame.at(3), frame.at(4)) {};
        // : src_rect{frame.at(0), frame.at(1), frame.at(2), frame.at(3)}, duration{frame.at(4)} {}

};

class Sprite
{
private:

    Vector2D position_;
    float    scale_;
    SDL_Rect src_rect_;
    SDL_Rect dest_rect_;
    int      z_index_;

    std::unordered_map<std::string, SDL_Texture*>	     textures_;
    std::unordered_map<std::string, std::vector<Frame>*> frames_;
    std::string	                                         current_animation_;
    std::vector<Frame>*	                                 animation_frames_;
    int	                                                 current_frame_;
    int	                                                 last_updated_;

public:

    Sprite();    // Set default values
    Sprite(SDL_Texture* texture, float scale, int z_index);

    void add_animation(std::string id, SDL_Texture* texture, std::vector<Frame>* frames = nullptr);

    void update(RenderManager* renderer, int dt);
    void render(SDL_Renderer* renderer);

    void animation(std::string id);
    void reset();

    // Setters

    void position(int x, int y);
    void x(int x);
    void y(int y);
    void move(int dx, int dy);
    void dimensions(int w, int h);
    void match_dimensions();
    void scale(float s);
    void set_src(int _x, int _y, int _w, int _h);
    void src_rect(SDL_Rect& source);
    void set_dest(int _x, int _y, int _w, int _h);
    void dest_rect(SDL_Rect& source);
    void z_index(int index);

    // Getters

    SDL_Rect* src_rect();
    SDL_Rect* dest_rect();
    Vector2D* position();
          int x();
          int y();
          int w();
          int h();
        float scale();
          int z_index();
    // void rotate(d) // Not yet

};
