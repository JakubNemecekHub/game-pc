#include "../PlayerManager.hpp"


PlayerManager::PlayerManager(LogManager* log)
    : log_{log} {}


bool PlayerManager::startUp(TextureManager* textures)
{
    log_->log("Starting Player Manager.");
    player = Player(textures);
    log_->log("Player Manager started.");
    return true;
}