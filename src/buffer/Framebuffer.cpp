//
// Created by Dmitri on 2026-11-18.
//

#include <GL/glew.h>
#include "Framebuffer.h"

Framebuffer::Framebuffer(GLFWwindow* window) {

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glGenFramebuffers(1, &framebufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);

    glGenRenderbuffers(1, &renderbufferObject);

    glGenTextures(1, &position);
    glBindTexture(GL_TEXTURE_2D, position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
    AssignParameters();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position, 0);

    glGenTextures(1, &distanceToCamera);
    glBindTexture(GL_TEXTURE_2D, distanceToCamera);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_FLOAT, 0);
    AssignParameters();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D, distanceToCamera, 0);

    glGenTextures(1, &normal);
    glBindTexture(GL_TEXTURE_2D, normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
    AssignParameters();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 2, GL_TEXTURE_2D, normal, 0);

    glGenTextures(1, &albedo);
    glBindTexture(GL_TEXTURE_2D, albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    AssignParameters();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 3, GL_TEXTURE_2D, albedo, 0);

    uint32_t attachments[4] = {
        GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
    };

    glDrawBuffers(4, attachments);

    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferObject);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Update(GLFWwindow* window) {

    glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferObject);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glBindTexture(GL_TEXTURE_2D, position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, distanceToCamera);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

}

void Framebuffer::Bind(GLFWwindow* window) {
    Update(window);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferObject);
}

void Framebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::AssignParameters() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}