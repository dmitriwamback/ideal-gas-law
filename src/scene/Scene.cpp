//
// Created by Dmitri on 2026-11-17.
//

#include "Scene.h"

#include "../shader/ShaderSources.h"

Scene::Scene() {
    softBody = std::unique_ptr<SoftBody>(nullptr);
    shader = Shader(DEFAULT_VERTEX_SHADER_SOURCE, DEFAULT_FRAGMENT_SHADER_SOURCE);
}

void Scene::Populate(Cube &cube) {
    cube.UpdateCollider();
    cubes.push_back(cube);
}

void Scene::SetSoftbody(SoftBody& body) {
    this->softBody = std::make_unique<SoftBody>(std::move(body));
}

void Scene::Render(const Camera& camera, double dt) {

    softBody->Step(dt, softBody->RecommendedSubsteps(dt), cubes);

    shader.Use();
    shader.SetMatrix4("projection", camera.projection);
    shader.SetMatrix4("lookAt", camera.lookAt);

    glm::mat4 identity = glm::mat4(1.0f);
    shader.SetMatrix4("model", identity);

    softBody->Render(shader);

    for (Cube cube : cubes) {
        cube.Render(shader);
    }
}
