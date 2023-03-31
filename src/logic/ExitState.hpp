#pragma once

#include "State.hpp"


class ExitState : public State
{
private:

    static ExitState self_;
    ExitState();

    Managers* managers_;

    Sprite* outro_;

public:
    static ExitState* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void input(SDL_Event event) override; 
    void update(int dt) override; 
    void render() override;

};