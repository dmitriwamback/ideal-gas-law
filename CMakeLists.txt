cmake_minimum_required(VERSION 4.3)
project(ideal_gas_law)

set(CMAKE_CXX_STANDARD 20)

add_executable(ideal_gas_law main.cpp
        src/scene/Renderer.cpp
        src/scene/Renderer.h
        src/shader/Shader.cpp
        src/shader/Shader.h
        src/objects/SoftBody.cpp
        src/objects/SoftBody.h
        src/physics/Collision.cpp
        src/physics/Collision.h
        src/WorldSettings.h
        src/MathUtil.h
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/scene/Camera.cpp
        src/scene/Camera.h
        src/shader/ShaderSources.h
        src/objects/Cube.cpp
        src/objects/Cube.h
        src/objects/Framebuffer.cpp
        src/objects/Framebuffer.h)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(ideal_gas_law PRIVATE
    glfw
    GLEW::GLEW
    glm::glm
)
