//
// Created by Dmitri on 2026-08-22.
//

#ifndef IDEAL_GAS_LAW_SOFTBODY_H
#define IDEAL_GAS_LAW_SOFTBODY_H
#include <vector>
#include <glm/vec2.hpp>

#include "../WorldSettings.h"
#include "../shader/Shader.h"

class SoftBody {
public:
    std::vector<glm::vec2> positions;
    std::vector<glm::vec2> velocities;
    std::vector<float> massPoints;

    std::vector<float> restEdgeLengths;
    std::vector<float> restAngles;
    float restArea;
    float nRT;

    SoftBody();

    void Step(WorldSettings worldSettings, float dt);
    //void Render(Shader shader);

private:
    void ApplyGravity(std::vector<glm::vec2>& forces, const glm::vec2& gravity);
    //void ApplyPressure(std::vector<glm::vec2>& forces);
    //void ApplyPerimeterSpring(std::vector<glm::vec2>& forces);
    //void ApplyBendingSpring(std::vector<glm::vec2>& forces);
    void ResolveGroundCollision(float groundY);
    //void Integrate(const std::vector<glm::vec2>& forces, float dt);

    float ComputeArea() const;
    static float SignedAngle(const glm::vec2& previous, const glm::vec2& current, const glm::vec2& next);

    uint32_t vertexArrayObject, vertexBufferObject, indexBufferObject;

    float pressureMultiplier = 1.0f;
    float edgeStiffness = 800.0f;
    float bendingStiffness = 40.0f;
    float damping = 0.995f;
    float restitution = 0.3f;
    float friction = 0.85f;
};


#endif //IDEAL_GAS_LAW_SOFTBODY_H
