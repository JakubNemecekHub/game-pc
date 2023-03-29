#include "../Visitor.hpp"

/*
    Include .hpp files here, because they are not included in the header.
*/
#include "../Item.hpp"
#include "../Door.hpp"
#include "../HotSpot.hpp"
#include "../../managers/TextManager.hpp"
#include "../../managers/PlayerManager.hpp"
#include "../../managers/RoomManager.hpp"


void Visitor::visit(Item* item, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    if ( right_click )  // Look.
    {
        std::string observation { item->get_observation() };
        text_->register_text(observation, x, y, COLOR::GREEN);
    }
    else                // Try to pick up. TO DO: move to Inventory
    {
        if ( player_->inventory.has_space() )
        {
            player_->inventory.add(item);
            room_->remove_item(item->id());
            text_->register_text(item->get_pick_observation(), x, y, COLOR::PURPLE);
        }
        else
        {
            text_->register_text("My Inventory is full.", x, y, COLOR::BEIGE);
        }
    }
}


void Visitor::visit(Door* door, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    if ( right_click )  // Look
    {
        std::string observation { door->get_observation() };
        text_->register_text(observation, x, y, COLOR::GREEN);
    }
    else
    {
        if ( door->locked() )
        {
            std::string key_id { door->key_id() };
            if ( player_->inventory.has_item(key_id) )
            {
                door->unlock();
                player_->inventory.remove(key_id);
                text_->register_text("Unlocked.", x, y, COLOR::PURPLE);
            }
            else
            {
                std::string observation { door->get_locked_observation() };
               text_->register_text(observation, x, y, COLOR::GREEN);
            }
        }
        else
        {
            text_->clean();
            room_->activate_room(door->target());
        }
    }
}


void Visitor::visit(HotSpot* hot_spot, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    std::string observation;
    if ( right_click )
    {
        observation = hot_spot->get_observation();
        text_->register_text(observation, x, y, COLOR::BEIGE);
    }
    else
    {
        observation = hot_spot->get_use_observation();
        text_->register_text(observation, x, y, COLOR::GREEN);
    }
}