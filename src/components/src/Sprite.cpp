#include "../Sprite.hpp"

Sprite::Sprite(RenderManager* renderer)
    : renderer_{renderer}, position_{0, 0}, scale_{1}, src_rect_{0, 0, 0, 0},
      dest_rect_{0, 0, 0, 0}, z_index_{0}, current_frame_{0},
      last_updated_{0} {}

// To create a Sprite for Text in TextManager
Sprite::Sprite(SDL_Texture* texture, float scale, int z_index)
    : scale_{scale}, z_index_{z_index}
{
    textures_.insert(std::make_pair("text", texture));
    current_animation_ = "text";
    src_rect_.x = 0;
    src_rect_.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &src_rect_.w, &src_rect_.h);
}

void Sprite::add_animation(std::string id, SDL_Texture* texture, std::vector<Frame>* frames)
{
    textures_.insert(std::make_pair(id, texture));
    if ( frames != nullptr ) frames_.insert(std::make_pair(id, frames));
}

void Sprite::animation(std::string id)
{
    // Select animation and its frames
    current_animation_ = id;        // Should first check if such an id exists in textures_
    // Reset the new animations
    current_frame_ = 0;
    last_updated_ = 0;

    try
    {
        animation_frames_ = frames_.at(id);
        src_rect_ = (*animation_frames_).at(current_frame_).src_rect_;
        // Must also set destination rectangle correctly,
        // position stays the same, but dimensions are different
        dest_rect_.w = src_rect_.w * scale_;
        dest_rect_.h = src_rect_.h * scale_;
    }
    catch (const std::out_of_range& e)
    {
        // "Static" animation -> really bad nomenclature
        SDL_QueryTexture(this->textures_.at(id), NULL, NULL, &src_rect_.w, &src_rect_.h);
        dest_rect_.w = src_rect_.w * scale_;
        dest_rect_.h = src_rect_.h * scale_;
    }
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
    Set src_rect values.
*/
void Sprite::set_src(int _x, int _y, int _w, int _h)
{
    src_rect_.x = _x;
    src_rect_.y = _y;
    src_rect_.w = _w;
    src_rect_.h = _h;
}


/*
    Copy the values from a given SDL_Rect source.
*/
void Sprite::src_rect(SDL_Rect& source)
{
    src_rect_.x = source.x;
    src_rect_.y = source.y;
    src_rect_.w = source.w;
    src_rect_.h = source.h;
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


// Get source rectangle.
SDL_Rect* Sprite::src_rect() { return &src_rect_; }
// Get source rectangle.
SDL_Rect* Sprite::dest_rect() { return &dest_rect_; }
// Get position
Vector2D* Sprite::position() { return &position_; }
// Get x position
int Sprite::x() { return dest_rect_.x; }
//Get y position
int Sprite::y() { return dest_rect_.y; }
// Get the Width
int Sprite::w() { return dest_rect_.w; }
// Get the Height
int Sprite::h() { return dest_rect_.h; }
// Get scale
float Sprite::scale() { return scale_; }
// Get the z-index
int Sprite::z_index() { return z_index_; }


void Sprite::update(RenderManager* renderer, int dt)
{
    if ( !frames_.empty() ) // If frames_ is empty do not update -> "static" sprite
    {
        last_updated_ += dt;
        auto frame = frames_.at(current_animation_)->at(current_frame_);
        if ( last_updated_ > frame.duration_ )
        {
            current_frame_ = (current_frame_ + 1) % (*animation_frames_).size();
            last_updated_ = 0;
            // Update source rectangle
            src_rect_.x = frames_.at(current_animation_)->at(current_frame_).src_rect_.x;
            src_rect_.y = frame.src_rect_.y;
            src_rect_.w = frame.src_rect_.w;
            src_rect_.h = frame.src_rect_.h;
            // Update destination rectangle
            dest_rect_.x = position_.x;
            dest_rect_.y = position_.y;
            dest_rect_.w = src_rect_.w * scale_;
            dest_rect_.h = src_rect_.h * scale_;
        }
    }
    renderer->submit(this);
}

void Sprite::reset()
{
    current_frame_ = 0;
    last_updated_ = 0;
    auto frame = frames_.at(current_animation_)->at(current_frame_);
    // Update source rectangle
    src_rect_.x = frames_.at(current_animation_)->at(current_frame_).src_rect_.x;
    src_rect_.y = frame.src_rect_.y;
    src_rect_.w = frame.src_rect_.w;
    src_rect_.h = frame.src_rect_.h;
}

void Sprite::render(SDL_Renderer* renderer)
{
    SDL_RenderCopyEx(renderer, textures_.at(current_animation_), &src_rect_, &dest_rect_, 0.0f, NULL, SDL_FLIP_NONE);
}

/*
    Match dimensions of destination rectangle to dimensions of
    the current Frame's source rectangle.
    (Current Frame's source rectangle should be at src_rect_
    of the base class.)
*/
void Sprite::match_dimensions()
{
    dest_rect_.w = static_cast<int>(src_rect_.w * scale_);
    dest_rect_.h = static_cast<int>(src_rect_.h * scale_);
}