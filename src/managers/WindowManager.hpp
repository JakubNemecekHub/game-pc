#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <iostream>

#include <SDL2/SDL.h>

class WindowManager
{
private:
    // Fields
    // Singleton
    static WindowManager* singleton_;
    // Normal fields
    SDL_Window* window_;
    const char* title = "Game";
    int width, height;
    // Methods
    // Constructor
    WindowManager() {};

public:
    // Fields
    bool isRunning;
    // SDL_Event event;

    // Methods
    static WindowManager* GetInstance();    // Singleton logic
    ~WindowManager() {};    // Descturctor that does nothing
    void startUp();         // Method that I can use instead of contructor
    void shutDown();        // Method that I can use instead of desctructor
    // void handleEvents();
    SDL_Window* window();
    void toggle_fullscreen();
    void close();
};

#endif // WINDOW_MANAGER_H