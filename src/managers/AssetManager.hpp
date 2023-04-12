#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <queue>

#include <SDL2/SDL.h>

#include "../components/Sprite.hpp"
#include "LogManager.hpp"
#include "RenderManager.hpp"

namespace fs = std::filesystem;


class AssetManager
{
private:

    LogManager* log_;
    fs::path path_ { "D:/Prog/game_project/game/res" };

    // Level 0
    std::map<std::string, SDL_Texture*>       textures_;
    std::map<std::string, SDL_Surface*>       bitmaps_;
    std::map<std::string, std::vector<Frame>> frames_;
    std::queue<fs::directory_entry>           items_meta_;
    std::queue<fs::directory_entry>           rooms_meta_;

    // Level 1
    std::map<std::string, Animation> animations_;
    std::map<std::string, Texture>   my_textures_;

    // Level 2
    std::map<std::string, Sprite> sprites_;

    // Private getters
    SDL_Texture* texture(std::string id);
    std::vector<Frame>* frames(std::string id);

public:

    AssetManager() {};
    AssetManager(LogManager* log);

    bool startUp(RenderManager* renderer);
    bool shutDown();

    // Public getters
    Sprite* sprite(std::string id);
    SDL_Surface* bitmap(std::string id);
    std::queue<fs::directory_entry> items_meta();
    std::queue<fs::directory_entry> rooms_meta();

};