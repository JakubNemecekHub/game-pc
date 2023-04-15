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
    ~HotSpot() {};

    std::string observation();
    std::string use_observation();

    void accept_click(Gameplay::GameplayState* handler, SDL_Event& event) override;
    void accept_over(Gameplay::GameplayState* handler, SDL_Event& event) override;
    void accept_drag(Gameplay::GameplayState* handler, SDL_Event& event) override;

};


class BitmapHotSpot : public HotSpot
{
public:

    BitmapHotSpot(YAML::Node data);
    void update(RenderManager* renderer, int dt) override;
    bool clicked(int x, int y) override;

};


class SpriteHotSpot : public HotSpot
{
private:

    Sprite* sprite_;
    Polygon click_area_;

public:

    SpriteHotSpot(YAML::Node data, AssetManager* assets);
    void update(RenderManager* renderer, int dt) override;
    bool clicked(int x, int y) override;

};