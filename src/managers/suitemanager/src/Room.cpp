#include "../Room.hpp"

#include <unordered_map>
#include <memory>           // unique_ptr
#include <string>
#include <charconv>         // std::string to number conversion

#include <SDL2/SDL.h>

#include "Ambient.hpp"
#include "GameObject.hpp"
#include "../../math_objects/PolygonObject.hpp"
#include "../../math/Polygon.hpp"
#include "../../RenderManager.hpp"


/*
    Room should take up the whole screen height - > set scale_full_h argument to true
    while loading texture. It should also be aligned to the middle of the screen.
    Important when texture width less that screen width. Rooms wider than screen size
    should scroll. [TO DO].
    <- Room must keep track of its destination (position and dimensions) on its own.
    Maybe this is true for all objects.
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
    for ( auto it = room_data["objects"].begin(); it != room_data["objects"].end(); ++it )
    {
        std::cout << it.key() << " | " << it.value() << "\n";
        // General
        std::string str_id {it.key()};
        Uint32 id;
        std::from_chars(str_id.data(), str_id.data() + str_id.size(), id);
        bool state {it.value().at("state")};
        std::vector<std::string> observations = it.value().at("look");
        // Ambient objects
        if ( it.value().at("type") == "ambient")
        {
            std::vector<std::string> use_observations = it.value().at("action-text");
            objects_ambient.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(id),
                                    std::forward_as_tuple(id, state, observations, use_observations));
        }
        else if ( it.value().at("type") == "door" )
        {
            std::string target {it.value().at("target")};
            bool locked {it.value().at("locked")};
            std::vector<std::string> locked_observations = it.value().at("locked-message");
            Uint32 key_id {it.value().at("key_id")};
            objects_door.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(id),
                                 std::forward_as_tuple(id, state, observations, target, locked, locked_observations, key_id));
        }
    }
    // load room ambient animations
    ambient.load(room_data["ambient"]);
}


Room::~Room()
{
    SDL_FreeSurface(click_map);
}


// Return true if given point in the room's walk area.
bool Room::point_in_polygon(int x, int y)
{
    return walk_area.point_in_polygon(x, y);
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

AmbientObject* Room::get_ambient_object(Uint32 id)
{
    if ( objects_ambient.find(id) != objects_ambient.end() )
    {
        return &objects_ambient.at(id);
    }
    return nullptr;
}

Door* Room::get_door(Uint32 id)
{
    if ( objects_door.find(id) != objects_door.end() )
    {
        return &objects_door.at(id);
    }
    return nullptr;
}


void Room::get_world_coordinates(int x, int y, int* world_x, int* world_y)
{
    *world_x = static_cast<int>((x - texture->dest_rect.x) / texture->scale);
    *world_y = static_cast<int>(y / texture->scale);
}
