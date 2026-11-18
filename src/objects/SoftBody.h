#ifndef IDEAL_GAS_LAW_SOFTBODY_H
#define IDEAL_GAS_LAW_SOFTBODY_H

#include <cstdint>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Cube.h"
#include "../WorldSettings.h"
#include "../MathUtil.h"
#include "../shader/Shader.h"

class SoftBody {
public:

    SoftBody(std::vector<Vertex> initial, std::vector<Triangle> tris, const Mixture& mix, const SoftbodyParams& params, double temperature);

    static SoftBody MakeIcosphere(int subdivisions, double radius, glm::dvec3 center, const Mixture& mix, const SoftbodyParams& params, double temperature);
    static SoftBody MakeEquilibriumIcosphere(int subdivisions, glm::dvec3 center, const Mixture& mix, const SoftbodyParams& params, double temperature);

    ~SoftBody();
    SoftBody(const SoftBody&) = delete;
    SoftBody& operator=(const SoftBody&) = delete;
    SoftBody(SoftBody&& o) noexcept;

    void Step(double dt, int substeps, std::vector<Cube>& cubes);
    int  RecommendedSubsteps(double dt) const;
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
    double RestVolume()  const { return V0; }
    double Pressure()    const { return P; }
    double Temperature() const { return T; }
    bool   Inverted()    const { return inverted; }
    double MaxStrain() const;   // max |L/L0 - 1| over all edges

private:

    void ResolveCollisions(const std::vector<Cube>& cubes);
    void Push(size_t i, const glm::dvec3& n, double depth);
    void UpdateTemperature(double h);
    void ComputeVolume();
    void ComputePressure(double h);   // h = 0: no damping update (for display)
    void ClearForces();
    void AddPressureForces();
    void AddSpringForces();
    void AddVertexForces();
    void Integrate(double h);
    void ResolveCollisions();

    void BuildEdges();
    void BuildNeighbors();
    void UpdateDerived();             // vertex mass, auto skin, bulk damping
    void InitBuffers();

    // static data
    std::vector<Vertex>   initialVertices;
    std::vector<Triangle> triangles;
    std::vector<uint32_t> indices;
    std::vector<Edge>     edges;
    std::vector<std::vector<int>> neighbors;
    std::vector<Vertex>   vertices;   // render buffer, derived from p

    // simulation state (double)
    std::vector<glm::dvec3> p, v, F;

    Mixture mix;
    SoftbodyParams params;

    double T, Ttarget, T0;
    double V0 = 0, V = 0, P = 0, dP = 0, Vprev = 0;
    double bulkDamp = 0, vertexMass = 0, restRadius = 0;
    bool   inverted = false;

    int grabbed = -1;
    glm::dvec3 grabTarget{0.0};

    uint32_t vertexArrayObject = 0;
    uint32_t vertexBufferObject = 0;
    uint32_t elementBufferObject = 0;
};

#endif