#pragma once

#include "../utils/Mouse.hpp"


class Visitor;
class TextManager;
class PlayerManager;
class RoomManager;

class GameObject
{
public:
    ~GameObject() {}
    virtual void accept(
        Visitor* visitor,
        TextManager* text_,
        PlayerManager* player_,
        RoomManager* room_,
        Mouse::click mouse
    ) = 0;
};
 