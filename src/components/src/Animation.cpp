#include "../Animation.hpp"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#include "json.hpp"
using json = nlohmann::json;

#include "../../managers/RenderManager.hpp"


// Constructor
Animation::Animation(Texture* _texture, std::vector<Frame> _frames, int _offset_x, int _offset_y, int _x, int _y)
    : texture{_texture}, current_frame{0}, last_updated{0}, frames{_frames},
      offset_x{_offset_x}, offset_y{_offset_y}
    {
        texture->set_position(_x, _y);
        reset();
    };

// Destructor
Animation::~Animation()
{
    // delete texture;
}


void Animation::reset()
{
    /*
        Sets current frame to the first frame of the animation
        and registers new Texture to renderer
    */
    current_frame = 0;
    texture->src_rect = frames[0].src_rect;
    texture->match_src_dimension(); // This should be handeled while loading the animation!
    RenderManager::GetInstance()->register_ambient_texture(texture);
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
        // TO DO: register new texture
        // RenderManager::GetInstance()->registerTexture(texture);
    }
}


std::unordered_map<std::string, Animation> Animation::load_animation(std::string file_name)
{
    /*
        Loads complete spritesheet with animation from a .anim file into an Animation object.
    */
    // Open file
    std::ifstream animation_file {file_name, std::ios::in};
    // Temporary variables
    std::string value {};   // The value loaded from the file , more clever name?
    std::unordered_map<std::string, Animation> animations {};
    std::string animation_name {};
    std::string sprite_sheet_name {};
    // Animation animation; // TO DO: add temporary Texture object and use it in the Animation construction
    Texture* texture;
    Frame frame; // TO DO: create a temporary vector of frames, populate it and then pass it to Animation constructor
    std::vector<Frame> frames;
    int x {0}, y {0};
    float scale {1.0f};
    int offset_x, offset_y;
    // Loading loop
    if ( animation_file.is_open() )
    {
        while ( !animation_file.eof() )
        {
            animation_file >> value;
            if ( value == "ANIMATION" )
            {
                animation_file >> animation_name;
            }
            if ( value == "FILENAME" )
            {
                animation_file >> sprite_sheet_name;
                texture = RenderManager::GetInstance()->load_texture(sprite_sheet_name.c_str());
            }
            if ( value == "POSITION" )
            {
                animation_file >> x;
                animation_file >> y;
            }
            if ( value == "SCALE" )
            {
                animation_file >> scale;
                texture->set_scale(scale);
            }
            if ( value == "OFFSET" )
            {
                animation_file >> offset_x;
                animation_file >> offset_y;
            }
            if ( value == "FRAME" )
            {
                animation_file >> frame.src_rect.x;
                animation_file >> frame.src_rect.y;
                animation_file >> frame.src_rect.w;
                animation_file >> frame.src_rect.h;
                animation_file >> frame.duration;
                frames.push_back(frame);
            }
            if ( value == "END" )
            {
                animations.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(animation_name),
                                    std::forward_as_tuple(texture, frames, offset_x, offset_y, x, y));
                frames.clear();
            }

        }
        // Close file after everything is loaded
        animation_file.close();
    }
    else
    {
        // File not opened
        std::cout << "File " << file_name << " cannot be opened." << std::endl;
    }
    return animations;
}


std::vector<Animation> Animation::load_animation_vector(json _json)
{
    std::vector<Animation> _animations;
    for (auto &animation : _json )
    {
        // Animation elements
        // > Texture
        Texture* texture;
        texture = RenderManager::GetInstance()->load_texture(animation["file"]);
        // > Scale
        float scale {animation["scale"]};
        texture->set_scale(scale);
        // > Frames
        Frame frame;
        std::vector<Frame> frames;
        for (auto &frame : animation["frames"] )
        {
            // x, y, w, h, duration
            frames.emplace_back(frame[0], frame[1], frame[2], frame[3], frame[4]);
        }
        // > offset
        int offset_x, offset_y;
        offset_x = animation["offset"][0];
        offset_y = animation["offset"][1];
        // > Position
        int x, y;
        x = animation["position"][0];
        y = animation["position"][1];
        // Emplace new Animation
        _animations.emplace_back(texture, frames, offset_x, offset_y, x, y);
    }
    return _animations;
}