//
// Created by Dmitri on 2026-11-18.
//

#ifndef IDEAL_GAS_LAW_FRAMEBUFFER_H
#define IDEAL_GAS_LAW_FRAMEBUFFER_H

#include <cstdint>
#include <glfw/glfw3.h>

class Framebuffer {
public:
    uint32_t position, distanceToCamera, normal, albedo;

    Framebuffer(GLFWwindow* window);
    void Update(GLFWwindow* window);
    void Bind(GLFWwindow* window);
    void Unbind();

private:
    uint32_t framebufferObject, renderbufferObject;
    void AssignParameters();
};


#endif //IDEAL_GAS_LAW_FRAMEBUFFER_H
