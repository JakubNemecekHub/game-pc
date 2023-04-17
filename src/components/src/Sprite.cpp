#include "../Sprite.hpp"

#include "../../managers/SerializationManager.hpp"
#include "../../managers/AssetManager.hpp"
#include "../../managers/RenderManager.hpp"


/*****************************************************************************************************
 *  Animation
*****************************************************************************************************/


Animation::Animation(std::string id, bool loop, AssetManager* assets)
    : flag_loop_{loop}, flag_finished_{false}, current_frame_{0}, last_updated_{0}
{
    texture_ = assets->texture(id);
    frames_ = assets->frames(id);
}


int Animation::w() { return (*frames_).at(current_frame_).src_rect_.w; }
int Animation::h() { return (*frames_).at(current_frame_).src_rect_.h; }
void Animation::update(int dt)
{
    // Only update animations that are supposed to loop;
    // or one time animation, that are not yet finished.
    if ( !flag_loop_ && (flag_loop_ || flag_finished_) ) return;

    last_updated_ += dt;

    int duration { (*frames_).at(current_frame_).duration_ };
    if ( last_updated_ <= duration ) return;

    // If the new frame would again be 0th frame then we finish the
    // one time animation.
    if ( current_frame_ + 1 == (*frames_).size() && !flag_loop_ )
    {
        flag_finished_ = true;
        return;
    }

    current_frame_ = (current_frame_ + 1) % (*frames_).size();
    last_updated_ = 0;

}
void Animation::reset()
{
    current_frame_ = 0;
    last_updated_ = 0;
}


void Animation::render(SDL_Renderer* renderer, SDL_Rect destination)
{
    SDL_Rect src_rect { (*frames_).at(current_frame_).src_rect_ };
    SDL_RenderCopyEx(renderer, texture_, &src_rect, &destination, 0.0f, NULL, SDL_FLIP_NONE);
}

void Animation::destroy() {};


/*****************************************************************************************************
 *  Texture
*****************************************************************************************************/

Texture::Texture(SDL_Texture* texture)
    : texture_{texture}
{
    src_rect_.x = 0;
    src_rect_.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &src_rect_.w, &src_rect_.h);
}
Texture::Texture(std::string id, AssetManager* assets)
{
    texture_ = assets->texture(id);
    src_rect_.x = 0;
    src_rect_.y = 0;
    SDL_QueryTexture(texture_, NULL, NULL, &src_rect_.w, &src_rect_.h);
}

int Texture::w() { return src_rect_.w; }
int Texture::h() { return src_rect_.h; }
void Texture::update(int dt) {}
void Texture::reset() {}


void Texture::render(SDL_Renderer* renderer, SDL_Rect destination)
{
    SDL_RenderCopyEx(renderer, texture_, &src_rect_, &destination, 0.0f, NULL, SDL_FLIP_NONE);
}

void Texture::destroy() { SDL_DestroyTexture(texture_); }

/*****************************************************************************************************
 *  Sprite
*****************************************************************************************************/

Sprite::Sprite(std::string id, RenderManager* renderer)
    : id_{id}, renderer_{renderer}, position_{0, 0}, scale_{1}, dest_rect_{0, 0, 0, 0},
      z_index_{0} {}

// To create a Sprite for Text in TextManager
Sprite::Sprite(SDL_Texture* texture, float scale, int z_index)
    : scale_{scale}, z_index_{z_index}
{
    depictions_.insert(std::make_pair("text", new Texture(texture)));
}


void Sprite::add_depiction(std::string id, AssetManager* assets)
{
    depictions_.emplace(std::piecewise_construct,
        std::forward_as_tuple(id),
        std::forward_as_tuple(std::make_unique<Texture>(id, assets))
    );
}

void Sprite::add_depiction(std::string id, bool loop, AssetManager* assets)
{
    depictions_.emplace(std::piecewise_construct,
        std::forward_as_tuple(id),
        std::forward_as_tuple(std::make_unique<Animation>(id, loop, assets))
    );
}


void Sprite::depiction(std::string id)
{
    // TO DO:: check if depiction exists.
    current_depiction_id_ = id;
    current_depiction_ = depictions_.at(id).get();
    current_depiction_->reset();    // last_updated_ and current_frame_ to 0
    dest_rect_.w = current_depiction_->w() * scale_;
    dest_rect_.h = current_depiction_->h() * scale_;
}


void Sprite::update(RenderManager* renderer, int dt)
{
    current_depiction_->update(dt);
    dest_rect_.x = position_.x;
    dest_rect_.y = position_.y;
    dest_rect_.w = current_depiction_->w() * scale_;
    dest_rect_.h = current_depiction_->h() * scale_;
    renderer->submit(this);
}


void Sprite::reset()
{
    current_depiction_->reset();
    dest_rect_.w = current_depiction_->w() * scale_;
    dest_rect_.h = current_depiction_->h() * scale_;
}

void Sprite::render(SDL_Renderer* renderer)
{
    current_depiction_->render(renderer, dest_rect_);
}

/*
    Match dimensions of destination rectangle to dimensions of
    the current Frame's source rectangle.
    (Current Frame's source rectangle should be at src_rect_
    of the base class.)
*/
void Sprite::match_dimensions()
{
    dest_rect_.w = static_cast<int>(current_depiction_->w() * scale_);
    dest_rect_.h = static_cast<int>(current_depiction_->h() * scale_);
}


/*
    Set Sprite's position to given coordinates.
    Also changes destination rectangle for renderer.
*/
void Sprite::position(int x, int y)
{
    position_.x = x;
    position_.y = y;
    dest_rect_.x = x;
    dest_rect_.y = y;
}
void Sprite::x(int x) {position_.x = x; dest_rect_.x = x;}
void Sprite::y(int y) {position_.y = y; dest_rect_.y = y;}


/*
    Move Sprite by given values from its current position.
    Also changes destination rectangle for renderer.
*/
void Sprite::move(int dx, int dy)
{
    int final_x { static_cast<int>(position_.x) + dx };
    int final_y { static_cast<int>(position_.y) + dy };
    position_.x = final_x;
    position_.y = final_y;
    dest_rect_.x = final_x;
    dest_rect_.y = final_y;
}

/*
    Set dimensions of destination rectangle to given values.
    Doesn't change scale_, so it can create discrepancies.
*/
void Sprite::dimensions(int w, int h)
{
    dest_rect_.w = w;
    dest_rect_.h = h;
}
/*
    Scale the destination rectangle.
*/
void Sprite::scale(float s)
{
    scale_ = s;
    dest_rect_.w *= s;
    dest_rect_.h *= s;
}


/*
    Set src_dest values.
*/
void Sprite::set_dest(int _x, int _y, int _w, int _h)
{
    dest_rect_.x = _x;
    dest_rect_.y = _y;
    dest_rect_.w = _w;
    dest_rect_.h = _h;
}


/*
    Copy the values from a given SDL_Rect destination.
*/
void Sprite::dest_rect(SDL_Rect& source)
{
    dest_rect_.x = source.x;
    dest_rect_.y = source.y;
    dest_rect_.w = source.w;
    dest_rect_.h = source.h;
}


/*
    Set z index.
*/
void Sprite::z_index(int index)
{
    z_index_ = index;
}


/*
    Scale the sprite to fit window's height.
*/
void Sprite::scale_full_h()
{
    renderer_->scale_full_h(this);
}


/*
    Center horizontally and vertically.
    Uses the renderer, the Sprite was created with.
*/
void Sprite::center()
{
    renderer_->center_horizontally(this);
    renderer_->center_vertically(this);
}
/*
    Center horizontally.
    Uses the renderer, the Sprite was created with.
*/
void Sprite::center_horizontally()
{
    renderer_->center_horizontally(this);
}
/*
    Center vertically.
    Uses the renderer, the Sprite was created with.
*/
void Sprite::center_vertically()
{
    renderer_->center_vertically(this);
}

// Destroy pointers
void Sprite::destroy() { current_depiction_->destroy(); }


/********* Serialization *********/

void Sprite::write(SerializationManager* io)
{
    io->write(id_);
    io->write(position_.x);
    io->write(position_.y);
    io->write(scale_);
    io->write(z_index_);
    io->write(current_depiction_id_);
}