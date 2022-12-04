#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "Animation.hpp"


class Sprite
{
private:

    std::unordered_map<std::string, Animation*>  animations_;
    Animation* current_animation_;

public:

    Sprite(std::unordered_map<std::string, Animation*> animations);

    void set_animation(std::string id);

};