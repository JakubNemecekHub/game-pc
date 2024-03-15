#pragma once

#include <string>
#include <vector>
#include <tuple>

#include <yaml-cpp/yaml.h>

#include "GameObject.hpp"

class State;
class AssetManager;
class RenderManager;
class Vector2D;


class Door : public GameObject
{
protected:

    bool                     locked_;
    std::string              target_;
    std::vector<std::string> observations_;
    std::vector<std::string> locked_observations_;
    std::string              key_id_;

    // Camera
    // Set Renderer's camera to these values after entering room through this door.
    Vector2D position_;
    float zoom_;

public:

    Door(YAML::Node data, AssetManager* assets);
    ~Door() {};

    void update(RenderManager* renderer, int dt) override;

    inline std::string target() { return target_;  }
    inline std::string key_id() { return key_id_;  }
    inline bool        locked() { return locked_;  }
    inline void        unlock() { locked_ = false; }
    std::string observation();
    std::string locked_observation();

    void accept_click(State* handler, Mouse::Status mouse) override;
    void accept_over(State* handler, Mouse::Status mouse) override;

    std::tuple<float, Vector2D> camera();

};
