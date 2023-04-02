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

};

class Depiction
{
public:
    ~Depiction() {};
    virtual int w() = 0;
    virtual int h() = 0;

    virtual void update(int dt) = 0;
    virtual void reset() = 0;
    virtual void render(SDL_Renderer* renderer, SDL_Rect destination) = 0; // This will need more info
};

class Animation : public Depiction
{
private:
    SDL_Texture* texture_;
    std::vector<Frame>* frames_;
    bool flag_loop_;
    bool flag_finished_;
    int current_frame_;
    int last_updated_;
public:
    Animation(SDL_Texture* texture, std::vector<Frame>* frames, bool loop);
    int w() override; 
    int h() override; 

    void update(int dt) override;
    void reset() override;
    void render(SDL_Renderer* renderer, SDL_Rect destination) override;
};

class Texture : public Depiction
{
private:
    SDL_Texture* texture_;
    SDL_Rect src_rect_;
public:
    Texture(SDL_Texture* texture);
    int w() override; 
    int h() override; 

    void update(int dt) override;
    void reset() override;
    void render(SDL_Renderer* renderer, SDL_Rect destination) override;
};

class Sprite 
{
private:

    RenderManager* renderer_; // The renderer that created the Sprite

    Vector2D position_;
    float    scale_;
    SDL_Rect dest_rect_;
    int      z_index_;
    
    std::unordered_map<std::string, Depiction*> depictions_;
    Depiction* current_depiction_;

public:

    Sprite(RenderManager* renderer);
    Sprite(SDL_Texture* texture, float scale, int z_index);

    void add_depiction(std::string id, Depiction* depiction);

    void update(RenderManager* renderer, int dt);
    void render(SDL_Renderer* renderer);

    void depiction(std::string id);
    void reset();

    // Setters

    void position(int x, int y);
    void x(int x);
    void y(int y);
    void move(int dx, int dy);
    void dimensions(int w, int h);
    void match_dimensions();
    void scale(float s);
    void set_dest(int _x, int _y, int _w, int _h);
    void dest_rect(SDL_Rect& source);
    void z_index(int index);
    void scale_full_h();
    void center();
    void center_horizontally();
    void center_vertically();

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
