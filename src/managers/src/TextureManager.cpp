#include "../TextureManager.hpp"

#include <iostream>

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
            room_background_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(base_folder + texture_file, renderer));
            Texture* texture = &room_background_.at(id);
            texture->match_src_dimension();
            renderer->scale_full_h(texture);                                // Fit to screen height.
            int x { (renderer->get_screen_width() - texture->w()) / 2 };    // Set default position.
            texture->set_position(x, 0);
            texture->set_z_index(0); //set z_index to 0, which is the layer reserved for Room background
            // Bitmap and its texture
            std::string bitmap_file { background["map"].as<std::string>() };
            SDL_Surface* surface = renderer->load_bitmap(base_folder + bitmap_file);
            room_bitmap_.insert(std::make_pair(id, surface));
            room_bitmap_texture_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(renderer->texture_from_surface(surface), 1, 2));
            room_bitmap_texture_.at(id).src_rect(texture->src_rect());
            room_bitmap_texture_.at(id).dest_rect(texture->dest_rect());
            room_bitmap_texture_.at(id).scale(texture->scale());
        }
        // Load Textures for "Ambient" animations
        YAML::Node ambient_data { data["suites"][0]["ambient"] };
        for ( auto& ambient : ambient_data )
        {
            std::string id           { ambient["id"].as<std::string>() };
            std::string texture_file { ambient["texture"].as<std::string>() };
            room_ambient_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(base_folder + texture_file, renderer));
        }
    }
    {   // Items
        std::string base_folder { data["items"]["base_folder"].as<std::string>() };
        YAML::Node item_data { data["items"]["textures"] };
        for ( auto& item : item_data )
        {
            std::string id           { item["id"].as<std::string>() };
            std::string texture_file { item["texture"].as<std::string>() };
            items_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(base_folder + texture_file, renderer));
        }
    }
    log_->log("Textures Loaded.");
    return true;
}


Texture* TextureManager::get_background(std::string id)
{
    if ( room_background_.find(id) != room_background_.end() )
    {
        return &room_background_.at(id);
    }
    return nullptr;
}


Texture* TextureManager::get_ambient(std::string id)
{
    if ( room_ambient_.find(id) != room_ambient_.end() )
    {
        return &room_ambient_.at(id);
    }
    return nullptr;
}


SDL_Surface* TextureManager::get_bitmap(std::string id)
{
    if ( room_bitmap_.find(id) != room_bitmap_.end() )
    {
        return room_bitmap_.at(id);
    }
    return nullptr;
}


Texture* TextureManager::get_bitmap_texture(std::string id)
{
    if ( room_bitmap_texture_.find(id) != room_bitmap_texture_.end() )
    {
        return &room_bitmap_texture_.at(id);
    }
    return nullptr;
}


Texture* TextureManager::get_item(std::string id)
{
    if ( items_.find(id) != items_.end() )
    {
        return &items_.at(id);
    }
    return nullptr;
}