#include "../Room.hpp"

#include <unordered_map>
#include <string>
#include <charconv>         // std::string to number conversion


/*
    Load animation data, specified in the yaml object
    into animations vector.
*/
using animation_counter = std::vector<Sprite*>::size_type;
void RoomAnimations::load(YAML::Node data, AssetManager* assets)
{
    animations_.resize(data.size()); // Set the size of the final animations vector
    for ( animation_counter i = 0; i < animations_.size(); i++ )
    {
        animations_.at(i) = assets->sprite(data[i]["id"].as<std::string>());
        animations_.at(i)->z_index(1);
        animations_.at(i)->scale(data[i]["scale"].as<float>());
        int x { data[i]["position"][0].as<int>() };
        int y { data[i]["position"][1].as<int>() };
        animations_.at(i)->position(x, y);
        // Reset animation
        animations_.at(i)->reset();
    }
}


/*
    Update each ambient animation and register it to the renderer
*/
void RoomAnimations::update(RenderManager* renderer, int dt)
{
   for ( auto &animation : animations_ )
   {
       animation->update(renderer, dt);
   }
}


/*
    Loads room's data based on info in the room data dictionary.
*/
Room::Room(YAML::Node data, RenderManager* renderer, ItemManager* items, AssetManager* assets)
{
    // Load room background Texture
    std::string id { data["id"].as<std::string>() };
    sprite_ = assets->sprite(id);
    renderer->scale_full_h(sprite_);
    renderer->center_horizontally(sprite_);
    // Load walk area polygon
    walk_area_.add_vertices(data["walkarea"].as<std::vector<std::vector<int>>>());
    walk_area_.visual.scale = sprite_->scale();
    walk_area_.visual.dx = sprite_->x();
    walk_area_.visual.dy = sprite_->y();
    // Load room click map
    click_map_ = assets->bitmap(id);
    // click_map_texture_ = assets->get_texture(id + "_bitmap");
    // Load HotSpots
    for ( auto& hot_spot : data["hot_spots"] )
    {
        hot_spots_.emplace(std::piecewise_construct,
                           std::forward_as_tuple(hot_spot["id"].as<Uint32>()),
                           std::forward_as_tuple(hot_spot));
    }
    // Load Doors
    for ( auto& door : data["doors"] )
    {
        doors_.emplace(std::piecewise_construct,
                       std::forward_as_tuple(door["id"].as<Uint32>()),
                       std::forward_as_tuple(door));
    }
    // Items
    for ( auto& item_data : data["items"] )
    {
        std::string id { item_data["id"].as<std::string>() };
        Item* item { items->get(id) };
        std::vector<int> position { item_data["position"].as<std::vector<int>>() };
        // scale item by its scale and also by the Room's scale.
        float item_scale { item_data["scale"].as<float>() };
        float room_scale { sprite_->scale() };
        item->sprite()->match_dimensions();
        item->sprite()->position(position[0] * room_scale + sprite_->x(), position[1] * room_scale);
        item->sprite()->scale(room_scale * item_scale);

        // Scale the click area Polygon in the same way
        item->click_area()->scale(item_scale);
        item->click_area()->move(position[0], position[1]);
        // Also scale its visual 
        item->click_area()->visual.scale = sprite_->scale();
        item->click_area()->visual.dx = sprite_->x();
        item->click_area()->visual.dy = sprite_->y();

        item->state(item_data["state"].as<bool>());
        item->lock(true);
        items_.insert(std::make_pair(id, item));
    }
    // load room ambient animations
    animations_.load(data["animations"], assets);
}


Room::~Room()
{
    SDL_FreeSurface(click_map_);
}


// Return true if given point in the room's walk area.
bool Room::walkable(int x, int y)
{
    int room_x, room_y;
    get_room_coordinates_(x, y, &room_x, &room_y);
    return walk_area_.point_in_polygon(room_x, room_y);
}


void Room::update(RenderManager* renderer, int dt)
{
    // Register background
    renderer->submit(sprite_);
    // Register visible items
    for ( auto& item : items_ )
    {
        if ( item.second->state() )
        {
            renderer->submit(item.second->sprite());
        }
    }
    // START OF DEBUG
    if ( visible_click_map_ ) renderer->submit(click_map_, sprite_->dest_rect());   // Register Click map if should be visible
    if ( visible_walk_area_ ) renderer->submit(&walk_area_);                        // Render Walk area if it should be visible
    if ( visible_item_click_map_ )                                                  // Render all items' click polygons if they should be visible
    {
        for ( auto& item : items_ )
        {
            renderer->submit(item.second->click_area());
        }
    }
    if ( visible_item_vector_ )
    {
        for ( auto& item : items_ )
        {
            renderer->submit(item.second->sprite()->position());
        }
    }
    // END OF DEBUG
    animations_.update(renderer, dt);
    update_items(renderer, dt);
}

/*
    Update each ambient animation and register it to the renderer
*/
void Room::update_items(RenderManager* renderer, int dt)
{
   for ( auto &item : items_ )
   {
       item.second->update(renderer, dt);
   }
}


/*
    Return the pixel colour value of the click map at a given location.

    The engine works with 32-bit bmp files. This is a bit overkill,only a
    few colours are needed for the click map. But lower bitrate seems to
    return unstable results (e.g. the same colour in different images
    lead to different results, probably based on other colour in the image)

    Probably:
    32bits = 2 ^ 32 = 4 294 967 296 colours
    8bits = 2 ^ 8 = 256 colours, this should be enough

*/
Uint32 Room::get_mapped_object_(int x, int y)
{
    int room_x, room_y;
    get_room_coordinates_(x, y, &room_x, &room_y);
    int bpp = click_map_->format->BytesPerPixel; // bytes per pixel, depends on loaded image
    Uint8 *p = (Uint8 *)click_map_->pixels + room_y * click_map_->pitch + room_x * bpp;
    switch (bpp)
    {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                return p[0] << 16 | p[1] << 8 | p[2];
            }
            else
            {
                return p[0] | p[1] << 8 | p[2] << 16;
            }
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;
    }
}

GameObject* Room::get_object(int x, int y)
{

    // Order: Item -> Door -> Hot Spot

    // Look for items.
    int room_x, room_y;
    get_room_coordinates_(x, y, &room_x, &room_y); // TO DO: destructuring
    for ( auto& item : items_ )
    {
        if ( item.second->clicked(room_x, room_y) && item.second->state() )
        {
            return item.second;
        }
    }
    // Look for objects on click map.
    Uint32 id { get_mapped_object_(x, y) };
    if ( doors_.find(id) != doors_.end() )
    {
        return &doors_.at(id);
    }
    if ( hot_spots_.find(id) != hot_spots_.end() )
    {
        return &hot_spots_.at(id);
    }
    return nullptr;
}

// HotSpot* Room::get_hot_spot(int x, int y)
// {
//     Uint32 id { get_mapped_object_(x, y) };
//     if ( hot_spots_.find(id) != hot_spots_.end() )
//     {
//         return &hot_spots_.at(id);
//     }
//     return nullptr;
// }

// Door* Room::get_door(int x, int y)
// {
//     Uint32 id { get_mapped_object_(x, y) };
//     if ( doors_.find(id) != doors_.end() )
//     {
//         return &doors_.at(id);
//     }
//     return nullptr;
// }


// Item* Room::get_item(int x, int y)
// {
//     int room_x, room_y;
//     get_room_coordinates_(x, y, &room_x, &room_y);
//     for ( auto& item : items_ )
//     {
//         if ( item.second->clicked(room_x, room_y) && item.second->state() )
//         {
//             return item.second;
//         }
//     }
//     return nullptr;
// }


void Room::remove_item(std::string id)
{
    items_.erase(id);
}

// void Room::item_activate(Uint32 id)
// {
//     items.at(id).set_state(true);
// }

// void Room::item_deactivate(Uint32 id)
// {
//     items.at(id).set_state(false);
// }


void Room::get_room_coordinates_(int x, int y, int* world_x, int* world_y)
{
    *world_x = static_cast<int>((x - sprite_->x()) / sprite_->scale());
    *world_y = static_cast<int>(y / sprite_->scale());
}
