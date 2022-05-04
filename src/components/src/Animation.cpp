#include "../Animation.hpp"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../../managers/RenderManager.hpp"


void Animation::reset()
{
    /*
        Sets current frame to the first frame of the animation
        and registers new Texture to renderer
    */
    current_frame = 0;
    texture->src_rect = frames[0].src_rect;
    texture->match_src_dimension(); // This should be handeled while loading the animation!
}


/* TO DO: test animation update code */
void Animation::update(int dt)
{
    /*
        Look at the current frame
        how long is it being shown? and how long is it supposed to be shown?
        add dt to the time the frame is being shown
        if new value is larger that this frame duration than move to the next frame with its shown time set to 0
            e.g set src_rect
        if new value is lesser than duration that do nothing
    */
    last_updated += dt;
    if ( last_updated > frames[current_frame].duration )
    {
        current_frame = (current_frame + 1) % frames.size();
        last_updated = 0;
        texture->src_rect = frames[current_frame].src_rect;
    }
    RenderManager::GetInstance()->register_object(texture.get());
}