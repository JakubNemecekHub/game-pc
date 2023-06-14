#pragma once

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>   // Uint32

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "../math/Polygon.hpp"
#include "../managers/AssetManager.hpp"


class Door : public GameObject
{
protected:

    bool                        state_;
    bool                        locked_;
    std::string                 target_;
    std::vector<std::string>    observations_;
    std::vector<std::string>    locked_observations_;
    std::string                 key_id_;

public:

    Door(YAML::Node data);
    ~Door() {};

    std::string target();
    bool        locked();
    void        unlock();
    std::string observation();
    std::string locked_observation();
    std::string key_id();

    void accept_click(State* handler, int x, int y, bool r) override;
    void accept_over(State* handler, int x, int y) override;
    void accept_drag(State* handler, SDL_Event& event) override;

};


class BitmapDoor : public Door
{
public:

    BitmapDoor(YAML::Node data);
    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

};


class SpriteDoor : public Door
{

    Sprite* sprite_;
    Polygon click_area_;

public:

    SpriteDoor(YAML::Node data, AssetManager* assets);
    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

};