#pragma once

#include <string>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>   // Uint32

#include "../../components/Texture.hpp"
#include "../../math/Polygon.hpp"


/*
    How to resolve actions?
    Look is easy. It always return text, so it gets priten on the screen.
    Use on the other hand does all srts of things.
*/


class GameObject
{
protected:
    Uint32 id;                              // Unique id, who will generate  it?
                                            // Could be uniqe only inside the room and correspond with a colour on the click map
                                            // Or it coud contain information about object type.
    std::string type_;
    bool state;                             // active/inactive
    std::vector<std::string> observations;  // List of observation to be returned from for look action
public:
    GameObject(Uint32 _id,
               std::string _type,
               bool _state,
               std::vector<std::string> _observations
              );
    ~GameObject() {};
    std::string type();
    std::string look();
    bool active();
    void set_state(bool _state);
    virtual std::string use() = 0;
};


/*
    Ambient object cannot be used, only looked at.

    Looking at them selects observation from general observations. Using them
    selects a observation from this class fiels use_observations.

    JSON:
    > type: ambient
    > keys: state, look, action-text
*/
class AmbientObject : public GameObject
{
private:
    const std::string _type {"ambient"};
    std::vector<std::string> use_observations;
public:
    AmbientObject(Uint32 _id,
                  bool _state,
                  std::vector<std::string> _observations,
                  std::vector<std::string> _use_observations
                 );
    ~AmbientObject() {};
    std::string use();
};


/*
    Using item will pick it up and place it into inventory.

    JSON:
    > type: item
    > keys: look
*/
class Item : public GameObject
{
private:
    std::unique_ptr<Texture> texture;
    Polygon click_area;
    std::string text_use;
public:
    Item(Uint32 _id,
         bool _state,
         std::vector<std::string> _observations,
         std::string _texture_file,
         std::vector<int> _position,
         float _scale,
         std::vector<std::vector<int>> _click_area,
         std::string _text_use
        );
    Item(Uint32 _id,
         bool _state,
         std::vector<std::string> _observations,
         std::string _texture_file,
         std::vector<int> _position,
         float _scale,
         Polygon _click_area,
         std::string _text_use
        );
    // Item(const Item& source) : GameObject(source) {};
    ~Item() {};
    std::string use();   // This is never used
    Polygon& get_click_area();
    Texture* get_texture_ptr();
    bool clicked(int x, int y);
};


/*
    Doors lead to other rooms. Can be in a locked or unlocked state. Unlocked
    by a specific item (or also by some game event)

    JSON:
    > type: door
    > keys: state, look, target, locked, key_id
*/
class Door : public GameObject
{
private:
    const std::string _type {"door"};
    std::string target;
    bool locked;
    std::vector<std::string> locked_observations;
    Uint32 key_id;
public:
    Door(Uint32 _id,
         bool _state,
         std::vector<std::string> _observations,
         std::string _target,
         bool _locked,
         std::vector<std::string> _locked_observations,
         Uint32 _key_id
        );
    ~Door() {};
    std::string use();  // This is never used
    std::string get_target();
    bool get_locked();
    std::string look_locked();
};


// /*
//     WIP
// */
// class GeneralObject : public GameObject
// {
// public:
//     GeneralObject();
//     ~GeneralObject() {};
//     std::string look() {};
//     std::string use() {};
// };