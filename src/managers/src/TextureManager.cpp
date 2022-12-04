#include "../TextureManager.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>

#include <yaml-cpp/yaml.h>


TextureManager::TextureManager(LogManager* log)
    : log_{log} {}


bool TextureManager::startUp(RenderManager* renderer)
{
    log_->log("Starting Texture Manager.");
    YAML::Node data = YAML::LoadFile(path_);
    {   // Suite
        std::string base_folder { data["suites"][0]["base_folder"].as<std::string>() };
        YAML::Node background_data { data["suites"][0]["rooms"] };
        for ( auto& background : background_data )
        {
            // Background
            std::string id           { background["id"].as<std::string>() };
            std::string texture_file { background["texture"].as<std::string>() };
            textures_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(base_folder + texture_file, renderer));
            // Background texture must be fitted to screen
            Texture* texture = &textures_.at(id);
            texture->match_src_dimension();
            renderer->scale_full_h(texture);                                // Fit to screen height.
            int x { (renderer->get_screen_width() - texture->w()) / 2 };    // Set default position.
            texture->set_position(x, 0);
            texture->set_z_index(0); //set z_index to 0, which is the layer reserved for Room background
            // Bitmap and its texture
            std::string bitmap_id { id + "_bitmap" };
            std::string bitmap_file { background["map"].as<std::string>() };
            SDL_Surface* surface = renderer->load_bitmap(base_folder + bitmap_file);
            bitmaps_.insert(std::make_pair(id, surface));
            textures_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(bitmap_id),
                    std::forward_as_tuple(renderer->texture_from_surface(surface), 1, 2));
            textures_.at(bitmap_id).src_rect(texture->src_rect());
            textures_.at(bitmap_id).dest_rect(texture->dest_rect());
            textures_.at(bitmap_id).scale(texture->scale());
        }
        // Load Rooms' Animations
        YAML::Node animations { data["suites"][0]["animations"] };
        for ( auto& animation : animations )
        {
            std::string id           { animation["id"].as<std::string>() };
            // Create Texture
            std::string texture_file { animation["texture"].as<std::string>() };
            textures_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(base_folder + texture_file, renderer));
            // Create Animation from just created texture
            animations_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(&textures_.at(id), animation["frames"].as<std::vector<std::vector<int>>>())
                    );
        }
    }
    {   // Items
        std::string base_folder { data["items"]["base_folder"].as<std::string>() };
        YAML::Node item_data { data["items"]["textures"] };
        for ( auto& item : item_data )
        {
            std::string id           { item["id"].as<std::string>() };
            std::string texture_file { item["texture"].as<std::string>() };
            textures_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(base_folder + texture_file, renderer));
        }
    }
    {   // Player
        std::string base_folder { data["player"]["base_folder"].as<std::string>() };
        YAML::Node animations { data["player"]["animations"] };
        for ( auto& animation : animations )
        {
            std::string id           { animation["id"].as<std::string>() };
            // Create Texture
            std::string texture_file { animation["texture"].as<std::string>() };
            textures_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(base_folder + texture_file, renderer));
            // Create Animation from just created texture
            animations_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(&textures_.at(id), animation["frames"].as<std::vector<std::vector<int>>>())
                    );
        }
        // // Create Sprites
        YAML::Node sprites { data["player"]["sprites"] };
        std::string sprite_id { sprites["id"].as<std::string>() };
        std::unordered_map<std::string, Animation*>  sprite_animations;
        for ( auto& animation_id : sprites["animations"].as<std::vector<std::string>>() )
        {
            sprite_animations.emplace(std::piecewise_construct,
                std::forward_as_tuple(animation_id),
                std::forward_as_tuple(&animations_.at(animation_id))
            );
        }
        sprites_.emplace(std::piecewise_construct,
            std::forward_as_tuple(sprite_id),
            std::forward_as_tuple(sprite_animations)
        );
    }
    log_->log("Assets Loaded.");
    return true;
}


Texture* TextureManager::get_texture(std::string id)
{
    if ( textures_.find(id) != textures_.end() )
    {
        return &textures_.at(id);
    }
    return nullptr;
}
SDL_Surface* TextureManager::get_bitmap(std::string id)
{
    if ( bitmaps_.find(id) != bitmaps_.end() )
    {
        return bitmaps_.at(id);
    }
    return nullptr;
}
Animation* TextureManager::get_animation(std::string id)
{
    if ( animations_.find(id) != animations_.end() )
    {
        return &animations_.at(id);
    }
    return nullptr;
}
Sprite* TextureManager::get_sprite(std::string id)
{
    if ( sprites_.find(id) != sprites_.end() )
    {
        return &sprites_.at(id);
    }
    return nullptr;
}
