//
// Created by Dmitri on 2026-08-22.
//

#ifndef IDEAL_GAS_LAW_RENDERER_H
#define IDEAL_GAS_LAW_RENDERER_H
#include <glm/vec2.hpp>

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "Camera.h"
#include "Scene.h"
#include "../WorldSettings.h"
#include "../objects/SoftBody.h"
#include "../buffer/Framebuffer.h"

class Renderer {
public:
    void Initialize();
    void GameLoop();

private:
    std::unique_ptr<Scene> scene;
    Camera camera;

    GLFWwindow *window;
    std::unique_ptr<Framebuffer> framebuffer;

    float deltaTime;
    float lastFrameTime;
    int frameCount;
    int fpsTimer;

    static void CursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
};

#endif //IDEAL_GAS_LAW_RENDERER_H
