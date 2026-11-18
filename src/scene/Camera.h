//
// Created by Dmitri on 2026-11-17.
//

#ifndef IDEAL_GAS_LAW_CAMERA_H
#define IDEAL_GAS_LAW_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 lookDirection;
    glm::vec3 upDirection;

    glm::mat4 projection, lookAt;

    float yaw = 0, pitch = 0;
    float speed = 40.0f;

    float lastMouseX, lastMouseY;
    int mouseButton = GLFW_MOUSE_BUTTON_RIGHT;

    Camera();

    void Update(glm::vec4 movement, float up, float down, GLFWwindow* window, float deltaTime);
    void UpdateLookAtMatrix();

    [[nodiscard]] glm::vec3 CalculateVelocity(glm::vec4 movement, float up, float down) const;
};


#endif //IDEAL_GAS_LAW_CAMERA_H
