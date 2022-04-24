#include "../RoomManager.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"
using json = nlohmann::json;

#include "../RenderManager.hpp"
#include "../../components/Animation.hpp"
#include "../../math_objects/PolygonObject.hpp"
#include "../../math/Polygon.hpp"


/************************************************************************
    Ambient
*************************************************************************/

/*
    Update each ambient animation and register it to the renderer
*/
void Ambient::update(int dt)
{
   for ( auto &animation : animations )
   {
       animation.update(dt);
       RenderManager::GetInstance()->register_object(animation.texture);
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
Room::~Room()
{
    delete texture;
    delete click_map;
}


void Room::update(int dt)
{
    // Register background
    RenderManager::GetInstance()->register_object(texture);
    // Register Click map if should be visible
    if ( visible_click_map )
    {
        RenderManager::GetInstance()->register_object(click_map_texture);
    }
    // Register Walk area if should be visible
    if ( visible_walk_area )
    {
        RenderManager::GetInstance()->register_object(screen_walk_area);
    }
    ambient->update(dt);
}


/*
    Create renderable click map texture from the click map bitmap.
    For testing purposes.
*/
void Room::create_click_map_texture()
{
    // could return bool about result
    // probably should check if click_map_texture doesn't already exists
    click_map_texture = new Texture;
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
    if ( visible_click_map && click_map_texture == nullptr )
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
    // could return bool about result
    // probably should check if screen_walk_area doesn't already exists
    Polygon* _polygon = new Polygon();
    *_polygon = Polygon(*walk_area);
    _polygon->scale(texture->scale);
    _polygon->move(texture->dest_rect.x, texture->dest_rect.y);
    screen_walk_area = new PolygonObject(_polygon);
    screen_walk_area->set_z_index(3);
}


/*
    Toggle rendering of walkarea.
*/
void Room::toggle_walk_area()
{
    visible_walk_area = !visible_walk_area;
    if ( visible_walk_area && screen_walk_area == nullptr )
    {
        create_screen_walk_area();
    }
}


Uint32 Room::get_mapped_object(int x, int y)
{
    int bpp = click_map->format->BytesPerPixel; // bytes per pixel, depeds on loaded image
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
    active_room = nullptr;
    load_rooms("suite-house");
    activate_room("Hall");
}


void RoomManager::shutDown()
{
    // delete ambient;
    // delete room;
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
            Collect necessary information and then emplace new Room into a unordered_map.
        */
        // Load room name
        std::string _name;
        _name = room_data["name"];
        // Load room background texture
        Texture* _texture;
        _texture = RenderManager::GetInstance()->load_texture(room_data["texture"]);
        // Fit to screen height.
        RenderManager::GetInstance()->scale_full_h(_texture);
        // Set default position.
        int x;
        x = (RenderManager::GetInstance()->get_screen_width() - _texture->dest_rect.w) / 2;
        _texture->set_position(x, 0);
        // set z_index to 0, which is the layer reserved for room background
        _texture->set_z_index(0);
        // Load walk area polygon
        Polygon* _walk_area = new Polygon;
        for ( auto &vertex : room_data["walkarea"] )
        {
            _walk_area->add_vertex(vertex[0], vertex[1]);
        }
        // Load room click map
        SDL_Surface* _click_map;
        _click_map = RenderManager::GetInstance()->load_bitmap(room_data["map"]);
        // Load doors
        std::unordered_map<Uint32, std::string> _doors;
        for ( auto &door : room_data["doors"] )
        {
            _doors.insert({{door[0], door[1]}});
        }
        // Load room actions
        std::unordered_map<Uint32, std::string> _actions;
        for ( auto &action : room_data["actions"] )
        {
            _actions.insert({{action[0], action[1]}});
        }
        // load room ambient animations
        std::vector<Animation> _animations;
        _animations = Animation::load_animation_vector(room_data["ambient"]);
        // Emplace new Room Object
        rooms.emplace(std::piecewise_construct,
                      std::forward_as_tuple(_name),
                      std::forward_as_tuple(_texture, _walk_area,_click_map, _doors, _actions, _animations));
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


void RoomManager::handle_click(int x, int y)
{
    // Convert viewport coordinates into world coordinates
    int world_x, world_y;
    world_x = (x - active_room->texture->dest_rect.x) / active_room->texture->scale;
    world_y = y / active_room->texture->scale;
    std::cout << "World coordinates: (" << world_x << ", " << world_y << ")" << std::endl;
    // Check walk area
    if ( active_room->walk_area->point_in_polygon(world_x, world_y) )
    {
        std::cout << "I am inside." << std::endl;
    }
    // Print out action
    Uint32 response;
    response = active_room->get_mapped_object(world_x, world_y);
    std::cout << response << " >> " << active_room->actions[response];
    if ( active_room->doors.find(response) != active_room->doors.end() )
    {
        // Change room
        std::cout << "| Changing room to " << active_room->doors[response];
        activate_room(active_room->doors[response]);
    }
    std::cout << std::endl;
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