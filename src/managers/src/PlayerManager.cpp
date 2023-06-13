#include "../PlayerManager.hpp"


/*
    Player
*/
 Player::Player(AssetManager* assets, std::string sprite_name, std::string start_animation_name)
    : position{0, 0}, destination{0, 0}, is_walking{false}, speed{3}, scale{1}
{
    const std::string id { sprite_name };
    sprite_ = assets->sprite(id);
    sprite_->depiction(start_animation_name); // sprite_->animation(ANIMATION::IDLE);
    // Set position
    sprite_->position(this->position.x, this->position.y);
    // Set dimensions
    sprite_->match_dimensions();
}


void Player::update(RenderManager* renderer, int dt)
{
    sprite_->update(renderer, dt);
}


void Player::walk(float x, float y)
{
    destination.x = x;
    destination.y = y;
    is_walking = true;
    sprite_->depiction("walk");
}


/*
    PlayerManager
*/
PlayerManager::PlayerManager(LogManager* log, AssetManager* assets, ItemManager* items)
    : log_{log}, assets_{assets}, items_{items} {}


bool PlayerManager::startUp(std::string sprite_name, std::string start_animation_name, std::string inventory_sprite_name)
{
    log_->log("Starting Player Manager.");
    // Create player
    player = Player(assets_, sprite_name, start_animation_name);
    inventory.startUp(items_);
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