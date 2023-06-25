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
        animations_.at(i).position(x, y);
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


Ambient* RoomAnimations::get_animation(float x, float y)
{
    for ( auto& animation : animations_ )
    {
        if ( animation.clicked(x, y) )
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
{
    // Load room background Texture
    std::string id { data["id"].as<std::string>() };
    sprite_ = assets->sprite(id);
    sprite_->scale_full_h();
    sprite_->center_horizontally();
    sprite_->z_index(0);
    // Load walk area polygon
    walk_area_.add_vertices(data["walkarea"].as<std::vector<std::vector<float>>>());
    walk_area_.visual.scale = sprite_->scale();
    walk_area_.visual.dx = sprite_->x();
    walk_area_.visual.dy = sprite_->y();

    // Load HotSpots
    for ( auto hot_spot : data["hot_spots"] )
    {
        std::string id { hot_spot["id"].as<std::string>() };
        objects_["hot_spots"].emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(std::make_shared<HotSpot>(hot_spot, assets, sprite_->x(), sprite_->y(), sprite_->scale()))
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
        std::vector<int> position { item_data["position"].as<std::vector<int>>() };
        float item_scale { item_data["scale"].as<float>() };
        float room_scale { sprite_->scale() };
        item->position(position[0] * room_scale + sprite_->x(), position[1] * room_scale);
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


// Transform coordinates to room's coordinates.
auto Room::relative_coordinates(float x, float y)
{
    struct result { float room_x; float room_y; };
    return result
    {
        (x - sprite_->x()) / sprite_->scale(),
        y / sprite_->scale()   
    };
}


// Return true if given point in the room's walk area.
bool Room::walkable(float x, float y)
{
    auto [room_x, room_y] = relative_coordinates(x, y);
    return walk_area_.point_in_polygon(room_x, room_y);
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
    // START OF DEBUG
    switch (debug_)
    {
    case DEBUG_STATE::WALK_AREA:
        renderer->submit(&walk_area_);
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


GameObject* Room::get_object(float x, float y)
{

    // Order: Item -> Door -> Hot Spot

    for ( auto& item : items_ )
        if ( item.second->clicked(x, y) && item.second->state() ) return item.second;
    for ( auto& door : objects_["doors"] )
        if ( door.second->clicked(x, y) && door.second->state() ) return door.second.get();
    for ( auto& hot_spot : objects_["hot_spots"] )
        if ( hot_spot.second->clicked(x, y) && hot_spot.second->state() ) return hot_spot.second.get();
    return nullptr;

}


void Room::remove_item(std::string id)
{
    if ( !items_.erase(id) ) objects_.erase(id);
}
