#pragma once

#include <map>
#include <string>

#include "LogManager.hpp"
#include "TextureManager.hpp"
#include "../components/Item.hpp"


class ItemManager
{
private:

    LogManager*                 log_;
    std::map<std::string, Item> items_;
    std::string                 path_ {"D:/Prog/game_project/game/res/items.yaml"};

public:

    ItemManager() {};
    ItemManager(LogManager* log);

    bool startUp(TextureManager* textures);
    bool shutDown();

    Item* get(std::string id);

};
