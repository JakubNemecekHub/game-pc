#pragma once

#include "State.hpp"


class ExitState : public State
{
private:

    static ExitState self_;
    ExitState();

    Managers* managers_;

    Sprite* outro_;

    // Helper

    void input_keyboard_(SDL_Event& event) override;
    void input_mouse_(SDL_Event& event) override;

public:
    static ExitState* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void update(int dt) override; 
    void render() override;

};