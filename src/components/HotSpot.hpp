#pragma once

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>   // Uint32

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "../math/Polygon.hpp"
#include "../managers/AssetManager.hpp"
#include "../managers/RenderManager.hpp"


class HotSpot : public GameObject
{
protected:

    std::vector<std::string>    observations_;
    std::vector<std::string>    use_observations_;

public:

    HotSpot(YAML::Node data);
    virtual ~HotSpot() {};

    std::string observation();
    std::string use_observation();

    void accept_click(State* handler, Mouse::Status mouse) override;
    void accept_over(State* handler, Mouse::Status mouse) override;
    void accept_drag(State* handler, Mouse::Status mouse) override;

    // TO DO: These are the same member function that Ambient, Item and Door/SpriteDoor must have.
    // How to avoid repetition?  
    virtual void x(float x) = 0;
    virtual void y(float y) = 0;
    virtual float x() = 0;
    virtual float y() = 0;
    virtual void move(float dx, float dy)  = 0;
    virtual void scale(float s) = 0;
    virtual float scale() = 0;

};


class BitmapHotSpot : public HotSpot
{
public:

    BitmapHotSpot(YAML::Node data);
    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

    // inline void accept_drag(State* handler, Mouse::Status mouse) override {}

    void x(float x) override {};
    void y(float y) override {};
    inline float x() override { return 0.0f; }
    inline float y() override { return 0.0f; }
    void move(float dx, float dy) override {};
    void scale(float s) override {};
    float scale() override { return 1.0f; }

};


class SpriteHotSpot : public HotSpot
{
private:

    Sprite* sprite_;
    Polygon click_area_;

public:

    SpriteHotSpot(YAML::Node data, AssetManager* assets);
    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

    void x(float x) override;
    void y(float y) override;
    inline float x() override { return sprite_->x(); }
    inline float y() override { return sprite_->y(); }
    void move(float dx, float dy) override;
    inline void scale(float s) override
    {
        sprite_->scale(s);
        click_area_.scale(s);
        click_area_.move((1.0f-s)*sprite_->x(), (1.0f-s)*sprite_->y()); // TO DO: implement vector arithmetic
    }
    inline float scale() override { return sprite_->scale(); }

};