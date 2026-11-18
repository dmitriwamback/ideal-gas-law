//
// Created by Dmitri on 2026-11-17.
//

#ifndef IDEAL_GAS_LAW_CUBE_H
#define IDEAL_GAS_LAW_CUBE_H
#include <vector>
#include "../WorldSettings.h"
#include "../shader/Shader.h"


class Cube {
public:
    Cube();

    glm::vec3 position, rotation, scale, color;

    std::vector<Vertex> GetTransformedVertices();
    std::vector<Triangle> BuildTriangleMesh();
    void Render(Shader shader);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t vertexArrayObject = 0, vertexBufferObject = 0, indexBufferObject = 0;
};


#endif //IDEAL_GAS_LAW_CUBE_H
