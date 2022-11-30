#include "../Ambient.hpp"

#include <vector>

#include "../../components/Animation.hpp"

using animation_counter = std::vector<Animation>::size_type;


/*
    Load animation data, specified in the yaml object
    into animations vector.
*/
void Ambient::load(YAML::Node data, TextureManager* textures)
{
    animations.resize(data.size()); // Set the size of the final animations vector
    for ( animation_counter i = 0; i < animations.size(); i++ )
    {
        // Texture
        animations.at(i).texture = textures->get_ambient(data[i]["id"].as<std::string>());
        // animations.at(i).texture = std::make_unique<Texture>(data[i]["texture"].as<std::string>());
        // > Texture's z_index to 1, which is the layer reserved for room ambient animations
        animations.at(i).texture->set_z_index(1);
        // > Scale
        animations.at(i).texture->scale(data[i]["scale"].as<float>());
        // > Position
        int x { data[i]["position"][0].as<int>() };
        int y { data[i]["position"][1].as<int>() };
        animations.at(i).texture->set_position(x, y);
        // Frames
        for (auto &frame : data[i]["frames"] )
        {
            // x, y, w, h, duration
            animations.at(i).frames.emplace_back(frame[0].as<int>(), frame[1].as<int>(), frame[2].as<int>(), frame[3].as<int>(), frame[4].as<int>());
        }
        // Offset
        // animations.at(i).offset_x = data[i]["offset"][0];
        // animations.at(i).offset_y = data[i]["offset"][1];
        // current_frame and last_updated
        animations.at(i).current_frame = 0;
        animations.at(i).last_updated = 0;
        // Reset animation
        animations.at(i).reset();
    }
}


/*
    Update each ambient animation and register it to the renderer
*/
void Ambient::update(RenderManager* renderer, int dt)
{
   for ( auto &animation : animations )
   {
       animation.update(renderer, dt);
   }
}