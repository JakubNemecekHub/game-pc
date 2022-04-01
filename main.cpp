#include <iostream>

#include "src/managers/WindowManager.hpp"
#include "src/managers/RenderManager.hpp"
#include "src/managers/ControlManager.hpp"
#include "src/managers/PlayerManager.hpp"
#include "src/managers/RoomManager.hpp"


/* Window Manager
    Just SDL window
*/
WindowManager* gWindowManager = WindowManager::GetInstance();
/* Render Manager
    Uses the window of Window Manager
    May work like this: Various game objects will register itself to be rendered.
    Rendered saves them to some stack (or queue?). Maybe will use different stacks
    for different priorities (background, foreground, HUD...).
*/
RenderManager* gRenderManager = RenderManager::GetInstance();
/* Control Manager
    Intercepts user inputs and evaluates them and passes them to appropriate managers
    left or right mouse click? Pass coordinates to current room.
    key pressed? Do e.g. show menu, inventory, close window
    Player <-> Room interaction
    Left Mouse Click - passed to room, result: possible action = walh, use, look,..... Pass it to Player.
*/
ControlManager* gControlManager = ControlManager::GetInstance();
/* Player Manager

*/
// PlayerManager* gPlayerManager = PlayerManager::GetInstance();
/* RoomManager
    Loads room, renders room, checks collisons and player action inside a room
*/
RoomManager* gRoomManager = RoomManager::GetInstance();
// Other possible systems
// PhysicsManager
// AnimationManager
//  Register Animation, play animation (e.g. advance frame), register frame in RenderManager
//  !! find better word instead of register
// TextureManager
//  Should Render Manager only render and this manager load and work with Textures?
// VideoManager
// MemoryManager
// FileSystemManager
//
// Concerning Game
// ...

int main(int argc, char* args[])
{
    std::cout << "Running" << std::endl;
    // start up engine systems in the correct order
    gWindowManager->startUp();
    gRenderManager->startUp();
    // gPlayerManager->startUp();
    gRoomManager->startUp();

    // Game loop
    while (gWindowManager->isRunning)
    {
        gControlManager->handleEvents();
        gRenderManager->render();
    }

    // shut down system in appropriate order (e.g. in reverse)
    gRenderManager->shutDown();
    gWindowManager->shutDown();

    return 0;
}