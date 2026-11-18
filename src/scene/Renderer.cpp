//
// Created by Dmitri on 2026-08-22.
//

#include "Renderer.h"

#include <iostream>
#include <ostream>
#include <sstream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::Initialize() {

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }

#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(1200, 800, "Softbody Simulation", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, CursorPositionCallback);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW init failed: " << glewGetErrorString(err) << std::endl;
    }
    glEnable(GL_DEPTH_TEST);

    double gasMassKg = 30.0;
    std::vector<Component> components = {
        {GasType::UraniumHexafluoride, 0.05},
        {GasType::HeliumGas, 0.05},
        {GasType::TungstenHexafluoride, 0.9}
    };
    Mixture m = makeMixture(components, gasMassKg);

    SoftbodyParams params;

    SoftBody softBody = SoftBody::MakeEquilibriumIcosphere(2, glm::dvec3(0), m, params, 273.0);

    const double dt = deltaTime;
    softBody.Step(dt, softBody.RecommendedSubsteps(dt));

    camera = Camera();
    scene = std::make_unique<Scene>();
    scene->SetSoftbody(softBody);


    Cube cube = Cube();
    cube.position = glm::vec3(0.0f, -10.25f, 0.0f);
    cube.scale = glm::vec3(4.0f, 0.25f, 4.0f);
    cube.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    scene->Populate(cube);
}

void Renderer::GameLoop() {

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        double currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        frameCount++;
        if (currentTime - fpsTimer >= 1.0) {
            std::stringstream ss;
            ss << "Softbody Simulation - FPS: " << frameCount;
            glfwSetWindowTitle(window, ss.str().c_str());
            frameCount = 0;
            fpsTimer += 1.0;
        }

        glm::vec4 movement;
        movement.x = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ?  1.0f : 0.0f;
        movement.y = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? -1.0f : 0.0f;
        movement.z = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ?  1.0f : 0.0f;
        movement.w = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? -1.0f : 0.0f;

        float up   = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ?  1.0f : 0.0f;
        float down = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ? -1.0f : 0.0f;

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        camera.Update(movement, up, down, window, deltaTime);
        camera.UpdateLookAtMatrix();

        scene->Render(camera, 1/120.0f);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void Renderer::CursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {

    auto* self = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

    if (glfwGetMouseButton(window, self->camera.mouseButton)) {
        float dy = ypos - self->camera.lastMouseY;
        float dx = xpos - self->camera.lastMouseX;

        self->camera.pitch -= dy * 0.005f;
        self->camera.yaw += dx * 0.005f;

        if (self->camera.pitch > 1.55f)  self->camera.pitch = 1.55f;
        if (self->camera.pitch < -1.55f) self->camera.pitch = -1.55f;

        self->camera.lookDirection = glm::normalize(glm::vec3(cos(self->camera.yaw) * cos(self->camera.pitch), sin(self->camera.pitch), sin(self->camera.yaw) * cos(self->camera.pitch)));
    }

    self->camera.lastMouseX = xpos;
    self->camera.lastMouseY = ypos;
}