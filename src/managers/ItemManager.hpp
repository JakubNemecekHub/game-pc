#pragma once

#include <map>
#include <string>


class LogManager;
class AssetManager;
class SerializationManager;
class Item;

class ItemManager
{
private:

    LogManager*                 log_;
    AssetManager*               assets_;
    SerializationManager*       io_;
    std::map<std::string, Item> items_;
    std::string                 path_ {"D:/Prog/game_project/game/res/items.yaml"};

public:

    ItemManager() {};
    ItemManager(LogManager* log, AssetManager* assets, SerializationManager* io);

    bool startUp();
    bool shutDown();

    Item* get(std::string id);

    // Serialization
    
    void save();
    void load();

};
