#include "../Room.hpp"

#include <charconv>         // std::string to number conversion

#include "../Sprite.hpp"
#include "../GameObject.hpp"
#include "../Item.hpp"
#include "../Door.hpp"
#include "../HotSpot.hpp"
#include "../../managers/RenderManager.hpp"
#include "../../managers/ItemManager.hpp"
#include "../../managers/AssetManager.hpp"


/*
    Load animation data, specified in the yaml object
    into animations vector.
*/
using animation_counter = std::vector<Sprite*>::size_type;
void RoomAnimations::load(YAML::Node data, AssetManager* assets)
{
    for ( animation_counter i = 0; i < data.size(); i++ )
    {
        std::string id { data[i]["id"].as<std::string>() }; // Id both of the Ambient object and its Sprite.
        bool state { data[i]["state"].as<bool>() };
        animations_.emplace_back(id, assets->sprite(id), state);
        animations_.at(i).z_index(1);
        animations_.at(i).scale(data[i]["scale"].as<float>());
        float x { data[i]["position"][0].as<float>() };
        float y { data[i]["position"][1].as<float>() };
        animations_.at(i).position(Vector2D{x, y});
    }
}


/*
    Update each ambient animation and register it to the renderer
*/
void RoomAnimations::update(RenderManager* renderer, int dt)
{
   for ( auto &animation : animations_ )
   {
       if ( animation.state() ) animation.update(renderer, dt);
   }
}


Ambient* RoomAnimations::get_animation(Vector2D position)
{
    for ( auto& animation : animations_ )
    {
        if ( animation.clicked(position) )
        {
            return &animation;
        }
    }
    return nullptr;
}


/*
    Loads room's data based on info in the room data dictionary.
*/
Room::Room(YAML::Node data, ItemManager* items, AssetManager* assets)
    : walk_area_{data["id"].as<std::string>(), data["walkarea"].as<std::vector<std::vector<float>>>()}
{
    // Load room background Texture
    std::string id { data["id"].as<std::string>() };
    sprite_ = assets->sprite(id);
    sprite_->z_index(0);
    // Load default camera
    camera_zoom_ = data["camera"]["zoom"].as<float>();
    // TO DO: load as Vector2D
    camera_position_.x = data["camera"]["position"].as<std::vector<float>>()[0];
    camera_position_.y = data["camera"]["position"].as<std::vector<float>>()[1];

    // Load HotSpots
    for ( auto hot_spot : data["hot_spots"] )
    {
        std::string id { hot_spot["id"].as<std::string>() };
        objects_["hot_spots"].emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(std::make_shared<HotSpot>(hot_spot, assets))
        );
    }

    // Load Doors
    for ( auto door : data["doors"] )
    {
        std::string id { door["id"].as<std::string>() };
        objects_["doors"].emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(std::make_shared<Door>(door, assets, sprite_->x(), sprite_->y(), sprite_->scale()))
        );
    }

    // Items
    for ( auto item_data : data["items"] )
    {
        std::string id { item_data["id"].as<std::string>() };
        Item* item { items->get(id) };
        item->set_use(true);
        item->state(item_data["state"].as<bool>());
        std::vector<int> position { item_data["position"].as<std::vector<int>>() };  // TO DO: load as Vector2D
        float item_scale { item_data["scale"].as<float>() };
        float room_scale { sprite_->scale() };
        item->position(Vector2D{position[0] * room_scale + sprite_->x(), position[1] * room_scale});
        item->scale(room_scale * item_scale);
        items_.insert(std::make_pair(id, item));
    }

    // load room ambient animations
    if ( data["animations"] )
    {
        size_t animation_count = data["animations"].size();
        animations_.reserve(animation_count);
        animations_.load(data["animations"], assets);
    }
}


// Return true if given point in the room's walk area.
bool Room::walkable(Vector2D position)
{
    return walk_area_.clicked(position);
}


void Room::update(RenderManager* renderer, int dt)
{
    // Register background
    renderer->submit(sprite_);
    for ( auto& item : items_ )
    {
        if ( item.second->state() ) item.second->update(renderer, dt);
    }
    for ( auto& object_map : objects_ )
    {
        for (auto& object : object_map.second )
        {
            if ( object.second->state() ) object.second->update(renderer, dt);
        }
    }
    animations_.update(renderer, dt);
    walk_area_.update(renderer, dt);
    // START OF DEBUG
    switch (debug_)
    {
    case DEBUG_STATE::WALK_AREA:
        walk_area_.show_attributes();
        break;
    case DEBUG_STATE::ITEM:
        for ( auto& item : items_ ) item.second->show_attributes();
        break;
    case DEBUG_STATE::HOT_SPOT:
        for ( auto& hot_spot : objects_["hot_spots"] ) hot_spot.second->show_attributes();
        break;
    case DEBUG_STATE::DOOR:
        for ( auto& door : objects_["doors"] ) door.second->show_attributes();
        break;
    default:
        break;
    }
    // END OF DEBUG
}


GameObject* Room::get_object(Vector2D position)
{

    // Order: Item -> Door -> Hot Spot -> Walkarea

    for ( auto& item : items_ )
        if ( item.second->clicked(position) && item.second->state() ) return item.second;
    for ( auto& door : objects_["doors"] )
        if ( door.second->clicked(position) && door.second->state() ) return door.second.get();
    for ( auto& hot_spot : objects_["hot_spots"] )
        if ( hot_spot.second->clicked(position) && hot_spot.second->state() ) return hot_spot.second.get();
    if ( walk_area_.clicked(position) && walk_area_.state() ) return &walk_area_;
    return nullptr;

}


void Room::remove_item(std::string id)
{
    if ( !items_.erase(id) ) objects_.erase(id);
}


void Room::save(SerializationManager* io)
{
    std::ranges::for_each(items_, [&](const auto& pair) { pair.second->write(io); });
    std::ranges::for_each(objects_, [&](auto& pair) {
        std::ranges::for_each(pair.second, [&](auto& object_pair) { object_pair.second->write(io); });
    });
    walk_area_.write(io);
}
