#pragma once

#include "../State.hpp"


class GameObject;

enum EDITOR_TARGET
{
    EMPTY,
    ITEM,
    HOT_SPOT,
    DOOR,
    AMBIENT,
    WALK_AREA
};


class Editor : public State
{
private:

    static Editor self_;
    Editor();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event& event) override;
    void input_mouse_(SDL_Event& event) override;

    // bool          mouse_down_;
    // EDITOR_TARGET target_ { EDITOR_TARGET::EMPTY };
    // GameObject*   selection_;

public:
    static Editor* get();

    // // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void update(int dt) override; 
    void render() override; 

};
