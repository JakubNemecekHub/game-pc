#include "../PlayerManager.hpp"


PlayerManager::PlayerManager(LogManager* log)
    : log_{log} {}


bool PlayerManager::startUp(AssetManager* assets)
{
    log_->log("Starting Player Manager.");
    player = Player(assets);
    log_->log("Player Manager started.");
    return true;
}