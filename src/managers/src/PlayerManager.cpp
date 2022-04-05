#include "../PlayerManager.hpp"

#include <iostream>

#include "../RenderManager.hpp"
#include "../../math/Vector2D.hpp"
#include "../../components/Transform.hpp"
#include "../../components/Animation.hpp"
// #include "../components/Sprite.hpp"


// ======= Player =======
// Player::Player()
//     : transform{}, destination{}, is_walking{}, speed{}, scale{};
// {

// }

// ======= PlayerManager =======
PlayerManager* PlayerManager::singleton_ = nullptr;

PlayerManager* PlayerManager::GetInstance()
{
    if ( singleton_ == nullptr )
    {
        singleton_ = new PlayerManager();
    }
    return singleton_;
}

void PlayerManager::startUp()
{
    load_player("../res/gfx/player_animation.anim");
    // RenderManager::GetInstance()->registerTexture(room->texture->texture);
}

void PlayerManager::shutDown()
{

}

void PlayerManager::load_player(const char* player_file)
{
    // Transform
    player->transform.position = Vector2D{200, 200};
    player->transform.destination = Vector2D{0, 0};
    player->transform.is_walking = false;
    player->transform.speed = 4;
    player->transform.scale = 15;
    // Sprite
    player->sprite.animations = Animation::load_animation(player_file);
    player->sprite.set_animation("idle");
}