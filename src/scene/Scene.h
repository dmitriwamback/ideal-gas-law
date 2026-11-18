//
// Created by Dmitri on 2026-11-17.
//

#ifndef IDEAL_GAS_LAW_SCENE_H
#define IDEAL_GAS_LAW_SCENE_H
#include "Camera.h"
#include "../objects/Cube.h"
#include "../objects/SoftBody.h"


class Scene {
public:
    Scene();
    std::unique_ptr<SoftBody> softBody;
    std::vector<Cube> cubes;
    Shader shader;

    void SetSoftbody(SoftBody& softBody);
    void Populate(Cube& cube);
    void Render(Camera camera, double dt);
};


#endif //IDEAL_GAS_LAW_SCENE_H
