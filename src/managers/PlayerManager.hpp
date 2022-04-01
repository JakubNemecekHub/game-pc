#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <iostream>

#include <SDL2/SDL.h>

#include "../components/Transform.hpp"
#include "../components/Sprite.hpp"

class Player
{
private:
public:
    Player() {};    // Empty Constructor
    // Fields
    Transform transform;    // Position, destination, speed, scale
    Sprite sprite;          // All the animations
    // Methods
};

class PlayerManager
{
private:
    static PlayerManager* singleton_;   // Singleton
    PlayerManager() {};                 // Empty Constructor
    Player* player;                     // The player object this Manager manages
public:
    static PlayerManager* GetInstance();
    void startUp();
    void shutDown();
    void load_player(const char* player_file);
};

#endif // PLAYER_MANAGER_H