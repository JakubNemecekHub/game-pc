#include "../GameObject.hpp"

#include <string>
#include <iostream>
#include <stdlib.h> // rand
#include <unordered_map>

/***********************************************************************************
 * GameObject
************************************************************************************/
GameObject::GameObject(Uint32 _id,
                       std::string _type,
                       bool _state,
                       std::vector<std::string> _observations
                      )
    : id{_id}, type_{_type}, state{_state}, observations{_observations} {};


std::string GameObject::type()
{
    return type_;
}


std::string GameObject::look()
{
    /*
        Simple approach: use rand() and modulo size and select that element.
        Might be biased, but other solutions are harder.
    */
   return observations[rand() % observations.size()];
}


/***********************************************************************************
 * AmbientObject
************************************************************************************/
AmbientObject::AmbientObject(Uint32 _id,
                  bool _state,
                  std::vector<std::string> _observations,
                  std::vector<std::string> _use_observations
                 )
    : GameObject(_id, "ambient", _state, _observations), use_observations{_use_observations} {};


std::string AmbientObject::use()
{
    int index;
    index = rand() % use_observations.size();
    return use_observations[index];
}


/***********************************************************************************
 * Door
************************************************************************************/
Door::Door(Uint32 _id,
           bool _state,
           std::vector<std::string> _observations,
           std::string _target,
           bool _locked,
           std::vector<std::string> _locked_observations,
           Uint32 _key_id
          )
    : GameObject(_id, "door", _state, _observations), target{_target}, locked{_locked}, locked_observations{_locked_observations}, key_id{_key_id} {};


std::string Door::use()
{
    return "What now?";
}

std::string Door::get_target()
{
    return target;
}
bool Door::get_locked()
{
    return locked;
}
std::string Door::look_locked()
{
    int index;
    index = rand() % locked_observations.size();
    return locked_observations[index];
}



/***********************************************************************************
 * Item
************************************************************************************/
Item::Item(Uint32 _id,
           bool _state,
           std::vector<std::string> _observations,
           std::string _texture_file,
           std::vector<int> _position,
           float _scale,
           std::vector<std::vector<int>> _click_area
          )
    : GameObject(_id, "item", _state, _observations), click_area{_click_area}
{
    // Crete texture
    texture = std::make_unique<Texture>(_texture_file);
    texture->set_position(_position[0], _position[1]);
    texture->set_scale(_scale);
}

std::string Item::use()
{
    return "Way over my head";
}