#pragma once

// class TextManager;
// class PlayerManager;
// class RoomManager;
class Visitor;

class GameObject
{
public:
    ~GameObject() {}
    virtual void accept(Visitor* visitor) = 0;
};
 