#include "../Animation.hpp"


Animation::Animation(Texture* texture, std::vector<std::vector<int>> frames)
    : texture_{texture}, current_frame_{0}, last_updated_{0}
{
    // Frames
    for (auto &frame : frames )
    {
        // x, y, w, h, duration
        frames_.emplace_back(frame[0], frame[1], frame[2], frame[3], frame[4]);
    }
}


Texture* Animation::texture() { return texture_; }

/*
    Scale destination rectangle based on source rectangle of current frame.
*/
void Animation::scale(float s)
{
    int width { static_cast<int>(frames_[current_frame_].src_rect.w * s) };
    int height { static_cast<int>(frames_[current_frame_].src_rect.h * s) };
    texture_->set_dimensions(width, height);
}


/*
    Sets current frame to the first frame of the animation
    and registers new Texture to renderer PROBABlY NOT
*/
void Animation::reset()
{
    current_frame_ = 0;
    last_updated_ = 0;
    texture_->src_rect(frames_[0].src_rect);
    // texture_->match_src_dimension(); // This should be handled while loading the animation!
}


/*
    Look at the current frame
    how long is it being shown? and how long is it supposed to be shown?
    add dt to the time the frame is being shown
    if new value is larger that this frame duration than move to the next frame with its shown time set to 0
        e.g set src_rect
    if new value is lesser than duration that do nothing
*/
void Animation::update(RenderManager* renderer, int dt)
{
    last_updated_ += dt;
    if ( last_updated_ > frames_[current_frame_].duration )
    {
        current_frame_ = (current_frame_ + 1) % frames_.size();
        last_updated_ = 0;
        texture_->src_rect(frames_[current_frame_].src_rect);
    }
    renderer->register_object(texture_);
}