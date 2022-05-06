#include "../RoomManager.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <memory>   // unique_ptr

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../LogManager.hpp"
#include "../RenderManager.hpp"
#include "../TextManager.hpp"
#include "../../components/Animation.hpp"
#include "../../math_objects/PolygonObject.hpp"
#include "../../math/Polygon.hpp"

using animation_counter = std::vector<Animation>::size_type;


/************************************************************************
    Ambient
*************************************************************************/

/*
    Load animation data, specified in the ambiet_data json object
    into animations vector.
*/
void Ambient::load(json ambient_data)
{
    // Set the size if the animations vector
    animations.resize(ambient_data.size()); // This causes a problem
    std::string _texture_file;
    for ( animation_counter i = 0; i < animations.size(); i++ )
    {
        // Texture
        _texture_file = ambient_data[i]["file"];
        animations.at(i).texture = std::make_unique<Texture>(_texture_file);
        // > Texture's z_index
        animations.at(i).texture->set_z_index(1);
        // > Scale
        float scale {ambient_data[i]["scale"]};
        animations.at(i).texture->set_scale(scale);
        // > Position
        int x, y;
        x = ambient_data[i]["position"][0];
        y = ambient_data[i]["position"][1];
        animations.at(i).texture->set_position(x, y);
        // Frames
        for (auto &frame : ambient_data[i]["frames"] )
        {
            // x, y, w, h, duration
            animations.at(i).frames.emplace_back(frame[0], frame[1], frame[2], frame[3], frame[4]);
        }
        // Offset
        animations.at(i).offset_x = ambient_data[i]["offset"][0];
        animations.at(i).offset_y = ambient_data[i]["offset"][1];
        // current_frame and last_updated
        animations.at(i).current_frame = 0;
        animations.at(i).last_updated = 0;
        // Resett animation
        animations.at(i).reset();
    }
}


/*
    Update each ambient animation and register it to the renderer
*/
void Ambient::update(int dt)
{
   for ( auto &animation : animations )
   {
       animation.update(dt);
    //    RenderManager::GetInstance()->register_object(animation.texture.get());
   }
}


/************************************************************************
    Room
*************************************************************************/

/*
    Room should take up the whole screen height - > set scale_full_h argument to true while loading texture.
    It should also be aligned to the middle of the screen. Important when texture width less that screen width. TO DO.
    Rooms wider than screen size should sroll. TO DO.
    <- Room must keep track of its destination (position and dimensions) on its own. Maybe this is true for all objects.
*/

/*
    Loads room's data based on info in the room data dictionary.
*/
void Room::load(json room_data)
{
    // Load room background texture
    // texture unique pointer is now nullptr
    std::string _texture_file = room_data["texture"];   // must create temporary string, because
    texture = std::make_unique<Texture>(_texture_file); // make_unique here doesn't accept json object
    // Fit to screen height.
    RenderManager::GetInstance()->scale_full_h(texture);
    // Set default position.
    int x;
    x = (RenderManager::GetInstance()->get_screen_width() - texture->dest_rect.w) / 2;
    texture->set_position(x, 0);
    // set z_index to 0, which is the layer reserved for room background
    texture->set_z_index(0);
    // Load walk area polygon
    for ( auto &vertex : room_data["walkarea"] )
    {
        walk_area.add_vertex(vertex[0], vertex[1]);
    }
    // Load room click map
    click_map = RenderManager::GetInstance()->load_bitmap(room_data["map"]);
    // Load doors
    for ( auto &door : room_data["doors"] )
    {
        doors.insert({{door[0], door[1]}});
    }
    // Load room actions
    for ( auto &action : room_data["actions"] )
    {
        actions.insert({{action[0], action[1]}});
    }
    // load room ambient animations
    // Zjistit počet animací
    // Změnit velikost animations vectoru
    // zalidnit vector
    ambient.load(room_data["ambient"]);
    // ambient.animations = Animation::load_animation_vector(room_data["ambient"]);
}

Room::~Room()
{
    SDL_FreeSurface(click_map);
}


void Room::update(int dt)
{
    // Register background
    RenderManager::GetInstance()->register_object(texture.get());
    // Register Click map if should be visible
    if ( visible_click_map )
    {
        RenderManager::GetInstance()->register_object(click_map_texture.get());
    }
    // Register Walk area if should be visible
    if ( visible_walk_area )
    {
        RenderManager::GetInstance()->register_object(screen_walk_area.get());
    }
    ambient.update(dt);
}


/*
    Create renderable click map texture from the click map bitmap.
    For testing purposes.
*/
void Room::create_click_map_texture()
{
    // could return bool about result
    // probably should check if click_map_texture doesn't already exists
    click_map_texture = std::make_unique<Texture>();
    click_map_texture->texture = RenderManager::GetInstance()->texture_from_surface(click_map);
    click_map_texture->set_src(texture->src_rect);
    click_map_texture->set_dest(texture->dest_rect);
    click_map_texture->set_scale(texture->scale);
    click_map_texture->set_z_index(2);
}


/*
    Toggle rendering of click map.
*/
void Room::toggle_click_map()
{
    visible_click_map = !visible_click_map;
    if ( visible_click_map && !click_map_texture )
    {
        create_click_map_texture();
    }
}


/*
    Create renderable PolygonObject from walk area math polygon.
    For testing purposes.
*/
void Room::create_screen_walk_area()
{
    screen_walk_area = std::make_unique<PolygonObject>(walk_area,
                                         texture->scale,
                                         texture->dest_rect.x,
                                         texture->dest_rect.y
                                        );
}


/*
    Toggle rendering of walkarea.
*/
void Room::toggle_walk_area()
{
    visible_walk_area = !visible_walk_area;
    if ( visible_walk_area && !screen_walk_area )
    {
        create_screen_walk_area();
    }
}


/*
    Return the pixel colour value of the click map at a given location.

    The engine works with 32-bit bmp files. This is a bit overkill,only a
    few colours are needed for the click map. But lower bitrate seems to
    return unstable results (e.g. the same colour in different images
    lead to different reults, probably based on other colour in the image)

    Probably:
    32bits = 2 ^ 32 = 4 294 967 296 colours
    8bits = 2 ^ 8 = 256 clours, this should be enough

*/
Uint32 Room::get_mapped_object(int x, int y)
{
    int bpp = click_map->format->BytesPerPixel; // bytes per pixel, depends on loaded image
    Uint8 *p = (Uint8 *)click_map->pixels + y * click_map->pitch + x * bpp;
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


/************************************************************************
    Room Manager
*************************************************************************/
RoomManager* RoomManager::singleton_ = nullptr;


RoomManager* RoomManager::GetInstance()
{
    if ( singleton_ == nullptr )
    {
        singleton_ = new RoomManager();
    }
    return singleton_;
}


void RoomManager::startUp()
{
    LogManager::GetInstance()->log_message("Starting Room Manager.");
    active_room = nullptr;
    load_rooms("suite-house");
    activate_room("Bedroom");
}


void RoomManager::shutDown()
{
    LogManager::GetInstance()->log_message("Shutting down Room Manager.");
    active_room = nullptr;
}


/*
    Load necessary room data. All information is stored in a .json file.
    This include:
    -> room background texture
    -> room hot-spots map
    -> room actions
    -> room ambient animations
    -> wlaking area
    TO DO: add
    -> items present in the room
*/
void RoomManager::load_rooms(std::string suite_file)
{

    // Open json file
    json suite_data;
    std::ifstream animation_json(path + suite_file + ".json");
    animation_json >> suite_data;

    for ( auto &room_data : suite_data["rooms"] )
    {
        /*
            Create empty Room in the rooms map and the use its load method.
        */
        std::string _name;
        _name = room_data["name"];
        rooms.emplace(std::piecewise_construct,
                      std::forward_as_tuple(_name),
                      std::forward_as_tuple());
        rooms.at(_name).load(room_data);
    }
}


void RoomManager::activate_room(std::string room_name)
{
    active_room = &rooms[room_name];
}


void RoomManager::update(int dt)
{
    active_room->update(dt);
}


void RoomManager::handle_click(int x, int y, bool right_click)
{
    // Convert viewport coordinates into world coordinates
    int world_x, world_y;
    world_x = static_cast<int>((x - active_room->texture->dest_rect.x) / active_room->texture->scale);
    world_y = static_cast<int>(y / active_room->texture->scale);
    std::cout << "Screen coordinates: (" << x << ", " << y << "), ";
    std::cout << "World coordinates: (" << world_x << ", " << world_y << ")" << std::endl;
    // Check walk area
    if ( active_room->walk_area.point_in_polygon(world_x, world_y) )
    {
        std::cout << "I am inside." << std::endl;
    }
    // Print out action
    Uint32 response;
    response = active_room->get_mapped_object(world_x, world_y);
    if ( response )
    {
        // If right click, then do look. e.g. do action
        if ( right_click )
        {
            TextManager::GetInstance()->register_text(active_room->actions[response], x, y);
        }
        // If left click then use, e.g. use door
        else
        {
            if ( active_room->doors.find(response) != active_room->doors.end() )
            {
                // Change room
                std::cout << "Changing room to " << active_room->doors[response] << std::endl;
                activate_room(active_room->doors[response]);
            }
        }
    }
}


/*
    Handle keyboars inputs concerning room an room management.
    b: Toggle renderinf of click map.
    p: Toggle rendreing of walk area.
*/
void RoomManager::handle_keyboard(std::string key)
{
    // Use enum?
    if ( key == "bitmap" )
    {
        active_room->toggle_click_map();
    }
    if ( key == "polygon" )
    {
        active_room->toggle_walk_area();
    }
}