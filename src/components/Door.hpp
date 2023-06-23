#pragma once

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "GameObject.hpp"

class State;
class AssetManager;
class RenderManager;


class Door : public GameObject
{
protected:

    bool                     locked_;
    std::string              target_;
    std::vector<std::string> observations_;
    std::vector<std::string> locked_observations_;
    std::string              key_id_;

public:

    Door(YAML::Node data, AssetManager* assets);
    Door(YAML::Node data, AssetManager* assets, float room_x, float room_y, float room_scale);
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
    void accept_drag(State* handler, Mouse::Status mouse) override;

};
