//
// Created by Dmitri on 2026-08-22.
//

#ifndef IDEAL_GAS_LAW_WORLDSETTINGS_H
#define IDEAL_GAS_LAW_WORLDSETTINGS_H
#include <glm/vec2.hpp>

struct WorldSettings {
    glm::vec2 gravity = glm::vec2(0.0f, -980.0f);
    float ground = 0.0f;
};

#endif //IDEAL_GAS_LAW_WORLDSETTINGS_H
