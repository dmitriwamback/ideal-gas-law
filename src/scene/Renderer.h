//
// Created by Dmitri on 2026-08-22.
//

#ifndef IDEAL_GAS_LAW_RENDERER_H
#define IDEAL_GAS_LAW_RENDERER_H
#include <glm/vec2.hpp>

#include "../WorldSettings.h"
#include "../objects/SoftBody.h"

class Renderer {
public:
    WorldSettings world;
    SoftBody softBody;
};


#endif //IDEAL_GAS_LAW_RENDERER_H
