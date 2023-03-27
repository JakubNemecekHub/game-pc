#include "../PlayerManager.hpp"


/*
    PlayerManager
*/
 Player::Player(AssetManager* assets)
    : position{0, 0}, destination{0, 0}, is_walking{false}, speed{3}, scale{1}
{
    const std::string id { "player" };
    sprite_ = assets->sprite(id);
    sprite_->animation("idle"); // sprite_->animation(IDLE);
    // Set position
    sprite_->position(this->position.x, this->position.y);
    // Set dimensions
    sprite_->match_dimensions();
}


void Player::update(RenderManager* renderer, int dt)
{
    sprite_->update(renderer, dt);
}


void Player::walk(int x, int y)
{
    destination.x = x;
    destination.y = y;
    is_walking = true;
    sprite_->animation("walk");
}


/*
    PlayerManager
*/
PlayerManager::PlayerManager(LogManager* log)
    : log_{log} {}


bool PlayerManager::startUp(AssetManager* assets)
{
    log_->log("Starting Player Manager.");
    player = Player(assets);
    log_->log("Player Manager started.");
    return true;
}


void PlayerManager::update(RenderManager* renderer, int dt)
{
    player.update(renderer, dt);
}