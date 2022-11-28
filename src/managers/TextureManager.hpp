#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>

#include "../components/Texture.hpp"
#include "LogManager.hpp"
#include "RenderManager.hpp"


class TextureManager
{
private:

    LogManager* log_;
    std::string path_ {"D:/Prog/game_project/game/res/textures.yaml"};

    // Room's background and "Ambient" animations, click bitmap
    std::map<std::string, Texture> room_background_;
    std::map<std::string, Texture> room_ambient_;
    std::map<std::string, SDL_Surface*> room_bitmap_;
    std::map<std::string, Texture> room_bitmap_texture_;
    // Items
    std::map<std::string, Texture> items_;
    // In future: Doors and their animations, Player an ther animation

public:

    TextureManager() {};
    TextureManager(LogManager* log);

    bool startUp(RenderManager* renderer);
    bool shutDown();

    Texture* get_background(std::string id);
    Texture* get_ambient(std::string id);
    SDL_Surface* get_bitmap(std::string id);
    Texture* get_bitmap_texture(std::string id);
    Texture* get_item(std::string id);

};