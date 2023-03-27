#pragma once

#include <yaml-cpp/yaml.h>

#include "LogManager.hpp"
#include "AssetManager.hpp"
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
    Player(AssetManager* assets);

    void update(RenderManager* renderer, int dt);
    void walk(int x, int y);

};


class PlayerManager
{
private:

    LogManager* log_;

public:

    Player      player;
    Inventory   inventory;

    PlayerManager() {};
    PlayerManager(LogManager* log);

    bool startUp(AssetManager* assets);
    bool shutDown();
    void update(RenderManager* renderer, int dt);

};