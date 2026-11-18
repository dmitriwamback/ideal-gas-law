//
// Created by Dmitri on 2026-11-17.
//

#ifndef IDEAL_GAS_LAW_SCENE_H
#define IDEAL_GAS_LAW_SCENE_H

#include "Camera.h"
#include "../buffer/Framebuffer.h"
#include "../objects/Cube.h"
#include "../objects/Quad.h"
#include "../objects/SoftBody.h"

class Scene {
public:
    Scene();
    std::unique_ptr<SoftBody> softBody;
    std::vector<Cube> cubes;
    Shader shader, quadShader, deferredRenderingShader;
    Quad quad;

    void SetSoftbody(SoftBody& softBody);
    void Populate(Cube& cube);
    void RenderScene(const Camera& camera, double dt);
    void RenderComposite(std::unique_ptr<Framebuffer>& framebuffer, const Camera& camera, GLFWwindow* window);
};


#endif //IDEAL_GAS_LAW_SCENE_H
