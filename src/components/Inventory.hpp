#pragma once

#include <array>
#include <string>

#include "../managers/LogManager.hpp"
#include "../managers/AssetManager.hpp"
#include "../managers/ItemManager.hpp"
#include "Item.hpp"
#include "Sprite.hpp"

class Item;


constexpr int INVENTORY_SIZE { 7 };


class Inventory
{
private:

    ItemManager* item_assets_;

    std::array<Item*, INVENTORY_SIZE> items_ { nullptr };
    int count_;

    bool visible_;
    Sprite* gui_frame_;

public:

    Inventory();
    void startUp(ItemManager* items);

    // GUI set up
    void ini_gui(AssetManager* assets, RenderManager* renderer_);

    void toggle_visibility();
    void show();
    void hide();
    void update(RenderManager* renderer, int dt);

     bool has_space();
     bool add(std::string id);
     bool has_item(std::string id);
    Item* get(std::string id);
     bool remove(std::string id);
    //  bool remove(int index);
    //  bool remove(Item* item);
     void print(LogManager* log);

};