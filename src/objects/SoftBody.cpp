//
// Created by Dmitri on 2026-08-22.
//

#include "SoftBody.h"

void SoftBody::Step(WorldSettings worldSettings, float dt) {
    std::vector<glm::vec2> forces(positions.size(), glm::vec2(0.0f));

    ApplyGravity(forces, worldSettings.gravity);
    //ApplyPressure(forces);
    //ApplyBendingSpring(forces);
    //Integrate(forces, dt);
    ResolveGroundCollision(worldSettings.ground);
}

float SoftBody::ComputeArea() const {
    float area = 0.0f;
    size_t n = positions.size();

    for (size_t i = 0; i < n; i++) {
        size_t j = (i + 1) % n;
        area += positions[i].x * positions[j].y - positions[j].x * positions[i].y;
    }

    return area / 2.0f;
}

float SoftBody::SignedAngle(const glm::vec2& previous, const glm::vec2& current, const glm::vec2& next) {

    glm::vec2 edge1 = previous - current;
    glm::vec2 edge2 = next - current;

    float cross = edge1.x * edge2.y - edge2.x * edge1.y;
    float dot = edge1.x * edge2.y + edge2.x * edge1.y;

    return std::atan2(cross, dot);
}

void SoftBody::ApplyGravity(std::vector<glm::vec2> &forces, const glm::vec2 &gravity) {
    for (size_t i = 0; i < positions.size(); i++) {
        forces[i] += gravity * massPoints[i];
    }
}

void SoftBody::ResolveGroundCollision(float groundY) {
    for (size_t i = 0; i < positions.size(); i++) {
        if (positions[i].y < groundY) {
            positions[i].y = groundY;
            if (velocities[i].y < 0.0f) {
                velocities[i].y = -velocities[i].y * restitution;
                velocities[i].x *= friction;
            }
        }
    }
}