//
// Created by Dmitri on 2026-11-17.
//

#include "Cube.h"
#include "../MathUtil.h"

#include <GL/glew.h>

Cube::Cube() {

    vertices = {
        Vertex({-1.0f, -1.0f, -1.0f},  {0, 0, -1}, {0, 0}),
        Vertex({ 1.0f,  1.0f, -1.0f},  {0, 0, -1}, {1, 1}),
        Vertex({ 1.0f, -1.0f, -1.0f},  {0, 0, -1}, {1, 0}),
        Vertex({ 1.0f,  1.0f, -1.0f},  {0, 0, -1}, {1, 1}),
        Vertex({-1.0f, -1.0f, -1.0f},  {0, 0, -1}, {0, 0}),
        Vertex({-1.0f,  1.0f, -1.0f},  {0, 0, -1}, {0, 1}),

        Vertex({-1.0f, -1.0f,  1.0f},  {0, 0, 1}, {0, 0}),
        Vertex({ 1.0f, -1.0f,  1.0f},  {0, 0, 1}, {1, 0}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {0, 0, 1}, {1, 1}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {0, 0, 1}, {1, 1}),
        Vertex({-1.0f,  1.0f,  1.0f},  {0, 0, 1}, {0, 1}),
        Vertex({-1.0f, -1.0f,  1.0f},  {0, 0, 1}, {0, 0}),

        Vertex({-1.0f,  1.0f,  1.0f},  {-1, 0, 0}, {1, 1}),
        Vertex({-1.0f,  1.0f, -1.0f},  {-1, 0, 0}, {1, 0}),
        Vertex({-1.0f, -1.0f, -1.0f},  {-1, 0, 0}, {0, 0}),
        Vertex({-1.0f, -1.0f, -1.0f},  {-1, 0, 0}, {0, 0}),
        Vertex({-1.0f, -1.0f,  1.0f},  {-1, 0, 0}, {0, 1}),
        Vertex({-1.0f,  1.0f,  1.0f},  {-1, 0, 0}, {1, 1}),

        Vertex({ 1.0f,  1.0f,  1.0f},  {1, 0, 0}, {1, 1}),
        Vertex({ 1.0f, -1.0f, -1.0f},  {1, 0, 0}, {0, 0}),
        Vertex({ 1.0f,  1.0f, -1.0f},  {1, 0, 0}, {1, 0}),
        Vertex({ 1.0f, -1.0f, -1.0f},  {1, 0, 0}, {0, 0}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {1, 0, 0}, {1, 1}),
        Vertex({ 1.0f, -1.0f,  1.0f},  {1, 0, 0}, {0, 1}),

        Vertex({-1.0f, -1.0f, -1.0f},  {0, -1, 0}, {0, 0}),
        Vertex({ 1.0f, -1.0f, -1.0f},  {0, -1, 0}, {1, 0}),
        Vertex({ 1.0f, -1.0f,  1.0f},  {0, -1, 0}, {1, 1}),
        Vertex({ 1.0f, -1.0f,  1.0f},  {0, -1, 0}, {1, 1}),
        Vertex({-1.0f, -1.0f,  1.0f},  {0, -1, 0}, {0, 1}),
        Vertex({-1.0f, -1.0f, -1.0f},  {0, -1, 0}, {0, 0}),

        Vertex({-1.0f,  1.0f, -1.0f},  {0, 1, 0}, {0, 0}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {0, 1, 0}, {1, 1}),
        Vertex({ 1.0f,  1.0f, -1.0f},  {0, 1, 0}, {1, 0}),
        Vertex({ 1.0f,  1.0f,  1.0f},  {0, 1, 0}, {1, 1}),
        Vertex({-1.0f,  1.0f, -1.0f},  {0, 1, 0}, {0, 0}),
        Vertex({-1.0f,  1.0f,  1.0f},  {0, 1, 0}, {0, 1}),
    };

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);

    color = glm::vec3(0.7f);

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    UpdateCollider();
}

void Cube::Render(Shader shader) {

    UpdateCollider();

    shader.Use();
    glm::mat4 model = ComputeModelMatrix(position, rotation, scale);

    shader.SetMatrix4("model", model);
    shader.SetVector3("color", color);

    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::UpdateCollider() {
    glm::dmat4 m(ComputeModelMatrix(position, rotation, scale));
    glm::dvec3 ax(m[0]), ay(m[1]), az(m[2]);
    colHalf = glm::dvec3(glm::length(ax), glm::length(ay), glm::length(az));
    colRot = glm::dmat3(ax / colHalf.x, ay / colHalf.y, az / colHalf.z);
    colCenter = glm::dvec3(m[3]);
}

bool Cube::Collide(const glm::dvec3& pt, double margin, double& depth, glm::dvec3& normal) const {

    glm::dvec3 local = glm::transpose(colRot) * (pt - colCenter);
    glm::dvec3 q = glm::abs(local) - colHalf;
    glm::dvec3 nLocal(0.0);
    double sdf;

    if (q.x > 0.0 || q.y > 0.0 || q.z > 0.0) {
        glm::dvec3 out = glm::max(q, glm::dvec3(0.0));
        sdf = glm::length(out);
        if (sdf >= margin) return false;
        nLocal = (out / sdf) * glm::dvec3(local.x >= 0 ? 1.0 : -1.0, local.y >= 0 ? 1.0 : -1.0, local.z >= 0 ? 1.0 : -1.0);
    }
    else {
        int a = (q.x > q.y) ? ((q.x > q.z) ? 0 : 2) : ((q.y > q.z) ? 1 : 2);
        nLocal[a] = local[a] >= 0.0 ? 1.0 : -1.0;
        sdf = q[a];
    }
    depth = margin - sdf;
    normal = glm::normalize(colRot * nLocal);
    return true;
}