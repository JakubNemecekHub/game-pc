#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "Serial.hpp"
#include "../math/Vector2D.hpp"


class RenderManager;
class AssetManager;
class SerializationManager;

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

    virtual ~Depiction() {};
    virtual Depiction* clone() = 0;
    virtual int w() = 0;
    virtual int h() = 0;

    virtual void update(int dt) = 0;
    virtual void reset() = 0;
    virtual void render(SDL_Renderer* renderer, SDL_FRect destination) = 0;
    virtual void destroy() = 0;
    
};

class Animation : public Depiction
{
private:

    SDL_Texture* texture_;
    const std::vector<Frame>* frames_;
    bool flag_loop_;
    bool flag_finished_;
    int current_frame_;
    int last_updated_;

public:

    Animation(std::string id, bool loop, AssetManager* assets);
    Animation(const Animation& source);
    Animation* clone() override;
    int w() override; 
    int h() override; 

    void update(int dt) override;
    void reset() override;
    void render(SDL_Renderer* renderer, SDL_FRect destination) override;
    void destroy() override;

};

class Texture : public Depiction
{
private:

    SDL_Texture* texture_;
    SDL_Rect src_rect_;

public:

    Texture(SDL_Texture* texture);
    Texture(std::string id, AssetManager* assets);
    Texture* clone() override;
    int w() override; 
    int h() override; 

    void update(int dt) override;
    void reset() override;
    void render(SDL_Renderer* renderer, SDL_FRect destination) override;
    void destroy() override;

};

class Sprite : public Serial
{
private:

    RenderManager* renderer_; // The renderer that created the Sprite

    std::string id_;
    Vector2D    position_;
    float       scale_;
    SDL_FRect   dest_rect_;
    int         z_index_;
    
    std::unordered_map<std::string, std::unique_ptr<Depiction>> depictions_;
    std::string current_depiction_id_;
    Depiction* current_depiction_;

public:

    Sprite(std::string id, RenderManager* renderer);
    Sprite(SDL_Texture* texture, float scale, int z_index);
    Sprite(const Sprite& source);

    void add_depiction(std::string id, AssetManager* assets);               // Add Texture depiction
    void add_depiction(std::string id, bool loop, AssetManager* assets);    // Add Animation depiction

    void update(RenderManager* renderer, int dt);
    void render(SDL_Renderer* renderer);

    void depiction(std::string id);
    void reset();

    // Setters

    void position(float x, float y);
    void x(float x);
    void y(float y);
    void move(float dx, float dy);
    void dimensions(float w, float h);
    void match_dimensions();
    void set_scale(float s);
    void scale(float s);
    void set_dest(float _x, float _y, float _w, float _h);
    void dest_rect(SDL_FRect& source);
    void z_index(int index);
    void scale_full_h();
    void center();
    void center_horizontally();
    void center_vertically();

    // Getters

    inline std::string id()         { return id_;          }
    inline   SDL_FRect* dest_rect() { return &dest_rect_;  }
    inline   Vector2D* position()   { return &position_;   }
    inline         float x()        { return dest_rect_.x; }
    inline         float y()        { return dest_rect_.y; }
    inline         float w()        { return dest_rect_.w; }
    inline         float h()        { return dest_rect_.h; }
    inline       float scale()      { return scale_;       }
    inline         int z_index()    { return z_index_;     }
    // void rotate(d) // Not yet

    void destroy();

    // Serialization

    void write(SerializationManager* io) override;

};
