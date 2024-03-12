#pragma once

// #include <yaml-cpp/yaml.h>

#include "LogManager.hpp"
#include "AssetManager.hpp"
#include "RenderManager.hpp"
#include "ItemManager.hpp"
#include "../components/Inventory.hpp"
#include "../math/Vector2D.hpp"


enum PLAYER_STATE
{
    NONE,
    IDLE,
    WALKING,
    ACTION,
};


class Player
{
private:

    Sprite* sprite_;
    Vector2D destination_;
    bool     is_walking_;
    float    speed_;

    // Position and scale are already part of Sprite. Now I need to keep them in two separate places.
    Vector2D position_;
    float   scale_;

public:

    Player() {};
    Player(AssetManager* assets, std::string sprite_name, std::string start_animation_name);

    void update(RenderManager* renderer, const int dt);
    void position(Vector2D position);    // Place player no matter what
    void set_scale(float s);
    void scale(float s);
    void walk(Vector2D position);        // Try to walk to given location

};


class PlayerManager
{
private:

    LogManager*    log_;
    RenderManager* renderer_;
    AssetManager*  assets_;
    ItemManager*   items_;

public:

    Player      player;
    Inventory   inventory;

    PlayerManager() {};
    PlayerManager(LogManager* log, AssetManager* assets, ItemManager* items);

    bool startUp(std::string sprite_name, std::string start_animation_name,
                 float x, float y, float s,
                 std::string inventory_sprite_name);
    bool shutDown();
    void update(RenderManager* renderer, int dt);

};
