#pragma once

#include <yaml-cpp/yaml.h>

#include "LogManager.hpp"
#include "TextureManager.hpp"
#include "../components/Animation.hpp"
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
    Player(TextureManager* textures)
        : position{0, 0}, destination{0, 0}, is_walking{false}, speed{3}, scale{1}
    {
        const std::string id { "player" };
        sprite_ = textures->get_sprite(id);
    }

    void walk(int x, int y)
    {
        destination.x = x;
        destination.y = y;
        is_walking = true;
        sprite_->set_animation("walk");
    }

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

    bool startUp(TextureManager* textures);
    bool shutDown();

};