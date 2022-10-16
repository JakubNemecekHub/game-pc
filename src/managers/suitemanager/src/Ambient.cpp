#include "../Ambient.hpp"

#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../../components/Animation.hpp"

using animation_counter = std::vector<Animation>::size_type;


/*
    Load animation data, specified in the ambient_data json object
    into animations vector.
*/
void Ambient::load(json ambient_data)
{
    animations.resize(ambient_data.size()); // Set the size of the final animations vector
    for ( animation_counter i = 0; i < animations.size(); i++ )
    {
        // Texture
        std::string texture_file { ambient_data[i]["file"] };
        animations.at(i).texture = std::make_unique<Texture>(texture_file);
        // > Texture's z_index to 1, which is the layer reserved for room ambient animations
        animations.at(i).texture->set_z_index(1);
        // > Scale
        float scale { ambient_data[i]["scale"] };
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
        // Reset animation
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
   }
}