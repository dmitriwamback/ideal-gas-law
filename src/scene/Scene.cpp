//
// Created by Dmitri on 2026-11-17.
//

#include "Scene.h"

#include "../shader/ShaderSources.h"

Scene::Scene() {
    softBody = std::unique_ptr<SoftBody>(nullptr);
    shader = Shader(DEFAULT_VERTEX_SHADER_SOURCE, DEFAULT_FRAGMENT_SHADER_SOURCE);
    quadShader = Shader(QUAD_VERTEX_SHADER_SOURCE, QUAD_FRAGMENT_SHADER_SOURCE);
    deferredRenderingShader = Shader(DEFERRED_VERTEX_SHADER_SOURCE, DEFERRED_FRAGMENT_SHADER_SOURCE);

    quad = Quad();
    quad.Initialize();
}

void Scene::Populate(Cube &cube) {
    cube.UpdateCollider();
    cubes.push_back(cube);
}

void Scene::SetSoftbody(SoftBody& body) {
    this->softBody = std::make_unique<SoftBody>(std::move(body));
}

void Scene::RenderScene(const Camera& camera, double dt) {

    softBody->Step(dt, softBody->RecommendedSubsteps(dt), cubes);

    deferredRenderingShader.Use();
    deferredRenderingShader.SetMatrix4("projection", camera.projection);
    deferredRenderingShader.SetMatrix4("lookAt", camera.lookAt);

    glm::mat4 identity = glm::mat4(1.0f);
    deferredRenderingShader.SetMatrix4("model", identity);

    softBody->Render(deferredRenderingShader);

    for (Cube cube : cubes) {
        cube.Render(deferredRenderingShader);
    }
}


void Scene::RenderComposite(std::unique_ptr<Framebuffer>& framebuffer, const Camera& camera, GLFWwindow* window) {

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quadShader.Use();

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glm::vec2 size = glm::vec2(width, height);

#if defined(__APPLE__)
    size = glm::vec2(width * 2, height * 2);
#endif

    quadShader.SetMatrix4("inverseProjection", glm::inverse(camera.projection));
    quadShader.SetMatrix4("inverseLookAt", glm::inverse(camera.lookAt));
    quadShader.SetVector3("cameraPosition", camera.position);
    quadShader.SetVector2("screenSize", size);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffer->albedo);
    quadShader.SetInt("scene", 0);

    quad.Render();
}