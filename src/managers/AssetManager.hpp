#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <queue>

#include <SDL2/SDL.h>

#include "../components/Sprite.hpp"
#include "LogManager.hpp"
#include "RenderManager.hpp"
#include "SerializationManager.hpp"

namespace fs = std::filesystem;


class AssetManager
{
private:

    LogManager* log_;
    RenderManager* renderer_;
    fs::path path_ { "D:/Prog/game_project/game/res" };

    // Level 0

    std::map<std::string, SDL_Texture*>       textures_;
    std::map<std::string, SDL_Surface*>       bitmaps_;
    std::map<std::string, std::vector<Frame>> frames_;
    std::queue<fs::directory_entry>           items_meta_;
    std::queue<fs::directory_entry>           rooms_meta_;

    // Level 1

    std::map<std::string, Sprite> sprites_;

    // Loading methods

    void load_png_(std::queue<fs::directory_entry>& paths);
    void load_bmp_(std::queue<fs::directory_entry>& paths);
    void load_frames_(std::queue<fs::directory_entry>& paths);
    void load_sprites_(std::queue<fs::directory_entry>& paths);
    void create_sprite_(YAML::Node sprite_data);

public:

    AssetManager() {};
    AssetManager(LogManager* log);

    bool startUp(RenderManager* renderer);
    bool shutDown();

    // Public getters

    SDL_Texture* texture(std::string id);
    Sprite* sprite(std::string id);
    SDL_Surface* bitmap(std::string id);
    std::vector<Frame>* frames(std::string id);
    std::queue<fs::directory_entry> items_meta();
    std::queue<fs::directory_entry> rooms_meta();

    // Serialization
    
    void save(SerializationManager* io);
    void load(SerializationManager* io);

};