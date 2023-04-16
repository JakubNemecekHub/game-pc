#include "../SerializationManager.hpp"

#include "../LogManager.hpp"
#include "../AssetManager.hpp"


SerializationManager::SerializationManager(LogManager* log, AssetManager* assets)
    : log_{log}, assets_{assets} {}


bool SerializationManager::startUp() { return true; }


bool SerializationManager::shutDown() { return true; }


bool SerializationManager::open_out()
{
    ofs.open(path_);
    if ( ofs.fail() )
    {
        log_->error("Error creating save file:", path_);
        return false;
    }
    return true;
}


bool SerializationManager::close_out()
{
    ofs.close();
    return true;
}


bool SerializationManager::open_in()
{
    ifs.open(path_);
    if ( ifs.fail() )
    {
        log_->error("Error opening save file:", path_);
        return false;
    }
    return true;
}


bool SerializationManager::close_in()
{
    ifs.close();
    return true;
}