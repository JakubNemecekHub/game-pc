#include "../AssetManager.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string_view>

#include <yaml-cpp/yaml.h>

#include "../utils/Files.hpp"

namespace fs = std::filesystem;


AssetManager::AssetManager(LogManager* log)
    : log_{log} {}

bool AssetManager::startUp(RenderManager* renderer)
{
    log_->log("Starting Asset Manager.");

    // Loop all the files in "res" folder and sort their paths into their respective stacks.
    
    std::queue<fs::directory_entry> png_paths;
    std::queue<fs::directory_entry> bmp_paths;
    std::queue<fs::directory_entry> frames_paths;
    std::queue<fs::directory_entry> sprites_paths;

    for (auto const& entry : std::filesystem::recursive_directory_iterator{path_})
    {
        fs::path name { fs::path(entry).filename()  };
        fs::path ext  { fs::path(entry).extension() };
             if ( ext == ".png" )                                      png_paths.push(entry);
        else if ( ext == ".bmp" )                                      bmp_paths.push(entry);
        else if ( ext == ".yaml" && is_animation_meta(name.string()) ) frames_paths.push(entry);
        else if ( ext == ".yaml" && is_item_meta(name.string()) )      items_meta_.push(entry);
        else if ( ext == ".yaml" && is_room_meta(name.string()) )      rooms_meta_.push(entry);
        else if ( ext == ".yaml" && is_sprites_meta(name.string()) )   sprites_paths.push(entry);
    }

    /*
        Process all the files in all the queues.
        Exceptions:
            items_meta_ queue will be handled by ItemManager.
            rooms_meta_ queue will be handled by RoomManager.
    */
    while ( !png_paths.empty() )
    {
        fs::directory_entry entry { png_paths.front() };
        std::string id { base_name(entry) };
        textures_.insert(std::make_pair(id, renderer->load_sdl_texture(entry.path().string())));
        log_->log("Loaded texture ", entry.path().string());
        png_paths.pop();
    }

    while ( !bmp_paths.empty() )
    {
        fs::directory_entry entry { bmp_paths.front() };
        std::string id { base_name(entry) };
        bitmaps_.insert(std::make_pair(id, renderer->load_bitmap(entry.path().string())));
        log_->log("Loaded bitmap ", entry.path().string());
        bmp_paths.pop();
    }

    while ( !frames_paths.empty() )
    {
        fs::directory_entry entry { frames_paths.front() };
        std::string id { base_name(entry) };
        YAML::Node data = YAML::LoadFile(entry.path().string());
        std::vector<std::vector<int>> frames_meta { data["frames"].as<std::vector<std::vector<int>>>() };
        std::vector<Frame> frames;
        for ( auto& frame_meta : frames_meta ) frames.emplace_back(frame_meta);
        frames_.insert(std::make_pair(id, frames));
        log_->log("Loaded frames ", entry.path().string());
        frames_paths.pop();
    }

    while ( !sprites_paths.empty() )
    {
        fs::directory_entry entry { sprites_paths.front() };
        std::string id { base_name(entry) };
        YAML::Node data = YAML::LoadFile(entry.path().string());
        for (auto& sprite : data)
        {
            std::string sprite_id { sprite["id"].as<std::string>() };
            sprites_.emplace(std::piecewise_construct,
                std::forward_as_tuple(sprite_id),
                std::forward_as_tuple(renderer));
            for ( auto& depiction : sprite["textures"] ) 
            {
                std::string depiction_id { depiction["id"].as<std::string>() };
                if ( !frames(depiction_id) )
                {
                    // Create Texture
                    my_textures_.emplace(std::piecewise_construct,
                        std::forward_as_tuple(depiction_id),
                        std::forward_as_tuple(texture(depiction_id)));
                    sprites_.at(sprite_id).add_depiction(depiction_id, &my_textures_.at(depiction_id));
                }
                else
                {
                    // Create Animation
                    bool loop { true };
                    if (YAML::Node loop_flag = depiction["loop"])
                    {
                        loop = loop_flag.as<bool>();
                    }
                    animations_.emplace(std::piecewise_construct,
                        std::forward_as_tuple(depiction_id),
                        std::forward_as_tuple(texture(depiction_id), frames(depiction_id), loop));
                    sprites_.at(sprite_id).add_depiction(depiction_id, &animations_.at(depiction_id));
                }
                // Set current depiction
            }
            sprites_.at(sprite_id).depiction(sprite["textures"][0]["id"].as<std::string>());
        }
        log_->log("Created Sprites", entry.path().string());
        sprites_paths.pop();
    }

    log_->log("Assets Loaded.");
    return true;
}

SDL_Texture* AssetManager::texture(std::string id)
{
    if ( textures_.find(id) != textures_.end() ) return textures_.at(id);
    return nullptr;
}
std::vector<Frame>* AssetManager::frames(std::string id)
{
    if ( frames_.find(id) != frames_.end() ) return &frames_.at(id);
    return nullptr;
}
SDL_Surface* AssetManager::bitmap(std::string id)
{
    if ( bitmaps_.find(id) != bitmaps_.end() ) return bitmaps_.at(id);
    return nullptr; 
}
Sprite* AssetManager::sprite(std::string id)
{
    if ( sprites_.find(id) != sprites_.end() ) return &sprites_.at(id);
    // else if no such id exists
    log_->error("Requested missing sprite id \"", id, "\"");
    return nullptr;
}
std::queue<fs::directory_entry> AssetManager::items_meta()
{
    return items_meta_;
}
std::queue<fs::directory_entry> AssetManager::rooms_meta()
{
    return rooms_meta_;
}


bool AssetManager::shutDown()
{
    for ( auto& texture : textures_ ) SDL_DestroyTexture(texture.second);
    for ( auto& bitmap : bitmaps_ )   SDL_FreeSurface(bitmap.second);
    return true;
}