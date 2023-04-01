#pragma once

#include "State.hpp"


class IntroState : public State
{
private:

    static IntroState self_;
    IntroState();

    Managers* managers_;

    Sprite* logo_;

public:
    static IntroState* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void input(SDL_Event& event) override; 
    void update(int dt) override; 
    void render() override;

};