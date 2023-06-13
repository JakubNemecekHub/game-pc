#pragma once

#include "State.hpp"


class IntroState : public State
{
private:

    static IntroState self_;
    IntroState();

    Managers* managers_;

    Sprite* logo_;

    // Helper

    void input_keyboard_(SDL_Event& event) override;
    void input_mouse_(SDL_Event& event) override;

public:

    static IntroState* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void update(int dt) override; 
    void render() override;

};