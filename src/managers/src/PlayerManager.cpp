#include "../PlayerManager.hpp"


/*
    PlayerManager
*/
 Player::Player(AssetManager* assets)
    : position{0, 0}, destination{0, 0}, is_walking{false}, speed{3}, scale{1}
{
    const std::string id { "player" };
    sprite_ = assets->get_sprite(id);
}


void Player::walk(int x, int y)
{
    destination.x = x;
    destination.y = y;
    is_walking = true;
    sprite_->set_animation("walk");
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