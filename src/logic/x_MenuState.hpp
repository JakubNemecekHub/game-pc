#pragma once

#include "State.hpp"

#include "../gui/Button.hpp"


class MenuState : public State
{
private:

    static MenuState self_;
    MenuState();

    Managers* managers_;

    Button button_new_game_;
    Button button_load_;
    Button button_exit_;

    // Helper

    void input_keyboard_(SDL_Event& event);
    void input_mouse_(SDL_Event& event);

public:

    static MenuState* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void update(int dt) override; 
    void render() override;

};