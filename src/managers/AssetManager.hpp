#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>

#include "../components/Texture.hpp"
#include "../components/Animation.hpp"
#include "../components/Sprite.hpp"
#include "LogManager.hpp"
#include "RenderManager.hpp"


class AssetManager
{
private:

    LogManager* log_;
    std::string path_ {"D:/Prog/game_project/game/res/textures.yaml"};

    std::map<std::string, Texture>      textures_;
    std::map<std::string, SDL_Surface*> bitmaps_;
    std::map<std::string, Animation>    animations_;
    std::map<std::string, Sprite>       sprites_;

public:

    AssetManager() {};
    AssetManager(LogManager* log);

    bool startUp(RenderManager* renderer);
    bool shutDown();

    Texture*        get_texture(std::string id);
    SDL_Surface*    get_bitmap(std::string id);
    Animation*      get_animation(std::string id);
    Sprite*         get_sprite(std::string id);

};