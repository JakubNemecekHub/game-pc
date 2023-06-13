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

    Sprite*     sprite_;
    Vector2D    position;
    Vector2D    destination;
    bool        is_walking;
    int         speed;
    int         scale;

public:

    Player() {};
    Player(AssetManager* assets, std::string sprite_name, std::string start_animation_name);

    void update(RenderManager* renderer, int dt);
    void walk(float x, float y);

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

    bool startUp(std::string sprite_name, std::string start_animation_name, std::string inventory_sprite_name);
    bool shutDown();
    void update(RenderManager* renderer, int dt);

};