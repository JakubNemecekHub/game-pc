#pragma once

#include <vector>

#include <yaml-cpp/yaml.h>

#include "Animation.hpp"
#include "../managers/RenderManager.hpp"
#include "../managers/TextureManager.hpp"


class Ambient
{
private:

    std::vector<Animation> animations;

public:

    Ambient() {};
    ~Ambient() {};

    void load(YAML::Node data, TextureManager* textures);
    void update(RenderManager* renderer, int dt);

};