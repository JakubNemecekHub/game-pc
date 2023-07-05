#include "../PlayerManager.hpp"


/************************************************************************************************************
    Player
***********************************************************************************************************/
 Player::Player(AssetManager* assets, std::string sprite_name, std::string start_animation_name)
    : position_{0, 0}, destination_{0, 0}, is_walking_{false}, speed_{3.0f}, scale_{1.0f}
{
    const std::string id { sprite_name };
    sprite_ = assets->sprite(id);
    sprite_->depiction(start_animation_name); // sprite_->animation(ANIMATION::IDLE);
    // Set position
    sprite_->position(this->position_.x, this->position_.y);
    // Set dimensions
    sprite_->match_dimensions();
}


void Player::update(RenderManager* renderer, const int dt)
{
    if ( is_walking_ )
    {
        const float half_width      { sprite_->w() / 2                              };  // This should be in the "middle of player's feet" -> No, this is x_offset
        const float y_offset        { sprite_->h() - 5                              };  // This must be a y-offset to the "feet" of player's sprite
        // These two should be given by sprite's offset. Offset is the position of a "walk point" on the sprite (E.g. the pixel, whose coordinates
        // are indicated by mouse click position.)
        const Vector2D destination  { destination_ - Vector2D{half_width, y_offset} };  // Destination of the player's sprite, calculated from the desired
                                                                                        // click coordinates and sprite's offset 
        const Vector2D route        { destination - position_                       };  // Total distance to the destination
              Vector2D step         { (destination - position_).unit() * speed_     };  // This tick' step 
        if ( step > route )
        {
            step = route;
            is_walking_ = false;
            sprite_->depiction("idle");
        }
        position_ += step;
        sprite_->position(position_.x, position_.y);
    }
    sprite_->update(renderer, dt);
}


void Player::position(float x, float y) { position_.x = x; position_.y = y; sprite_->position(x, y); }
void Player::set_scale(float s) { scale_ = s; sprite_->set_scale(s); }
void Player::scale(float s) { scale_ *= s; sprite_->scale(s); }


void Player::walk(float x, float y)
{
    destination_.x = x;
    destination_.y = y;
    is_walking_ = true;
    sprite_->depiction("walk");
}


/************************************************************************************************************
    PlayerManager
************************************************************************************************************/
PlayerManager::PlayerManager(LogManager* log, AssetManager* assets, ItemManager* items)
    : log_{log}, assets_{assets}, items_{items} {}


bool PlayerManager::startUp(std::string sprite_name, std::string start_animation_name,
                            float x, float y, float s,
                            std::string inventory_sprite_name)
{
    log_->log("Starting Player Manager.");
    // Create player
    player = Player(assets_, sprite_name, start_animation_name);
    inventory.startUp(items_);
    player.position(x, y);
    player.set_scale(s);
    // Initialize inventor's GUI
    inventory.ini_gui(assets_, renderer_, inventory_sprite_name);
    log_->log("Player Manager started.");
    return true;
}


void PlayerManager::update(RenderManager* renderer, int dt)
{
    player.update(renderer, dt);
    inventory.update(renderer, dt);
}