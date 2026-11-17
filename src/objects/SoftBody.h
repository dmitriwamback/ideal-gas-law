//
// Created by Dmitri on 2026-08-22.
//

#ifndef IDEAL_GAS_LAW_SOFTBODY_H
#define IDEAL_GAS_LAW_SOFTBODY_H
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../WorldSettings.h"
#include "../MathUtil.h"
#include "../shader/Shader.h"

class SoftBody {
public:
    SoftBody(std::vector<Vertex> initial, std::vector<Triangle> tris, const Mixture& mix, const SoftbodyParams& params, double T0);

    static SoftBody MakeIcosphere(int subdivisions, double radius, glm::dvec3 center, const Mixture& mix, const SoftbodyParams& params, double T0);

    void Step(double dt, int substeps);
    void Reset();
    void Render(Shader& shader);
    void SetTemperatureTarget(double T) { Ttarget = T; }
    void SetMixture(const Mixture& m);
    void Grab(int vertex, glm::dvec3 target);
    void Release() { grabbed = -1; }

    void UpdateVertices();
    const std::vector<Vertex>& Vertices() const { return vertices; }
    const std::vector<Triangle>& Triangles() const { return triangles; }
    double Volume()      const { return V; }
    double Pressure()    const { return P; }
    double Temperature() const { return T; }

private:

    void UpdateTemperature(double h);
    void ComputeVolumeAndPressure();
    void ClearForces();
    void AddPressureForces();
    void AddSpringForces();
    void AddVertexForces();
    void Integrate(double h);
    void ResolveCollisions();

    void BuildEdges();
    void BuildNeighbors();

    std::vector<Vertex> initialVertices;
    std::vector<Triangle> triangles;
    std::vector<Edge> edges;
    std::vector<std::vector<int>> neighbors;
    double V0 = 0;

    std::vector<glm::dvec3> p, v, F;
    double vertexMass = 0;

    Mixture mix;
    double T, Ttarget;
    double V = 0, P = 0, dP = 0;

    std::vector<Vertex> vertices;

    SoftbodyParams params;
    int grabbed = -1;
    glm::dvec3 grabTarget{0.0};

    uint32_t vertexArrayObject  = 0;
    uint32_t vertexBufferObject = 0;
    uint32_t elementBufferObject = 0;
};


#endif //IDEAL_GAS_LAW_SOFTBODY_H
