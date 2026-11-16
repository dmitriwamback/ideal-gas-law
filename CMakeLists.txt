cmake_minimum_required(VERSION 4.3)
project(ideal_gas_law)

set(CMAKE_CXX_STANDARD 20)

add_executable(ideal_gas_law main.cpp
        src/Renderer.cpp
        src/Renderer.h
        src/shader/Shader.cpp
        src/shader/Shader.h
        src/objects/SoftBody.cpp
        src/objects/SoftBody.h
        src/physics/Pressure.cpp
        src/physics/Pressure.h
        src/physics/Collision.cpp
        src/physics/Collision.h
        src/objects/Constraints.cpp
        src/objects/Constraints.h
        src/WorldSettings.h
        src/MathUtil.h)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(ideal_gas_law PRIVATE
    glfw
    GLEW::GLEW
    glm::glm
)
