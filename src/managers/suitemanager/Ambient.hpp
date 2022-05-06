#pragma once

#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../../components/Animation.hpp"


class Ambient
{
private:
    // Fields
    std::vector<Animation> animations;
public:
    // Constructor
    Ambient() {};
    // Destructor
    ~Ambient() {};
    // Methods
    void load(json ambient_data);
    void update(int dt);
};