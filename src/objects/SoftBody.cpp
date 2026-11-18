#include "SoftBody.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace {
    constexpr double MIN_VOLUME = 1e-12;
    constexpr double PI = 3.14159265358979323846;

    uint64_t EdgeKey(int a, int b) {
        if (a > b) std::swap(a, b);
        return (static_cast<uint64_t>(static_cast<uint32_t>(a)) << 32) | static_cast<uint32_t>(b);
    }

    int AddMidpoint(int i, int j, std::vector<glm::dvec3>& positions, std::unordered_map<uint64_t, int>& cache) {
        uint64_t key = EdgeKey(i, j);
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;

        int index = static_cast<int>(positions.size());
        positions.push_back(glm::normalize((positions[i] + positions[j]) * 0.5));
        cache[key] = index;
        return index;
    }

    void OrientTriangle(Triangle& tri, const std::vector<glm::dvec3>& pos, const glm::dvec3& center) {
        glm::dvec3 a = pos[tri.a], b = pos[tri.b], c = pos[tri.c];
        glm::dvec3 normal = glm::cross(b - a, c - a);
        glm::dvec3 centroid = (a + b + c) / 3.0;
        if (glm::dot(normal, centroid - center) < 0.0) std::swap(tri.b, tri.c);
    }

    void BuildIcosphereMesh(int subdivisions, double radius, const glm::dvec3& center, std::vector<Vertex>& outVerts, std::vector<Triangle>& outTris) {
        const double t = (1.0 + std::sqrt(5.0)) / 2.0;

        std::vector<glm::dvec3> positions = {
            {-1, t, 0}, {1, t, 0}, {-1, -t, 0}, {1, -t, 0},
            {0, -1, t}, {0, 1, t}, {0, -1, -t}, {0, 1, -t},
            {t, 0, -1}, {t, 0, 1}, {-t, 0, -1}, {-t, 0, 1}
        };
        for (glm::dvec3& q : positions) q = glm::normalize(q);

        std::vector<Triangle> tris = {
            {0,11,5}, {0,5,1}, {0,1,7}, {0,7,10}, {0,10,11},
            {1,5,9}, {5,11,4}, {11,10,2}, {10,7,6}, {7,1,8},
            {3,9,4}, {3,4,2}, {3,2,6}, {3,6,8}, {3,8,9},
            {4,9,5}, {2,4,11}, {6,2,10}, {8,6,7}, {9,8,1}
        };

        for (int it = 0; it < subdivisions; ++it) {
            std::unordered_map<uint64_t, int> cache;
            std::vector<Triangle> refined;
            refined.reserve(tris.size() * 4);
            for (const Triangle& tri : tris) {
                int a = AddMidpoint(tri.a, tri.b, positions, cache);
                int b = AddMidpoint(tri.b, tri.c, positions, cache);
                int c = AddMidpoint(tri.c, tri.a, positions, cache);
                refined.push_back({tri.a, a, c});
                refined.push_back({tri.b, b, a});
                refined.push_back({tri.c, c, b});
                refined.push_back({a, b, c});
            }
            tris = std::move(refined);
        }

        outVerts.clear();
        outVerts.reserve(positions.size());
        for (const glm::dvec3& u : positions) {
            Vertex vtx{};
            vtx.position = glm::vec3(center + u * radius);
            vtx.normal = glm::vec3(u);
            vtx.uv = glm::vec2(0.0f);
            outVerts.push_back(vtx);
        }
        outTris = std::move(tris);
    }

    double UnitIcosphereVolume(int subdivisions) {
        std::vector<Vertex> verts;
        std::vector<Triangle> tris;
        BuildIcosphereMesh(subdivisions, 1.0, glm::dvec3(0.0), verts, tris);
        double sum = 0.0;
        for (const Triangle& t : tris) {
            glm::dvec3 a(verts[t.a].position), b(verts[t.b].position), c(verts[t.c].position);
            sum += glm::dot(a, glm::cross(b, c)) / 6.0;
        }
        return std::abs(sum);
    }
}


SoftBody::SoftBody(std::vector<Vertex> initial, std::vector<Triangle> tris, const Mixture& m, const SoftbodyParams& prm, double temperature) : initialVertices(std::move(initial)), triangles(std::move(tris)), mix(m), params(prm), T(temperature), Ttarget(temperature), T0(temperature) {
    if (initialVertices.empty()) throw std::invalid_argument("SoftBody requires vertices.");
    if (triangles.empty())       throw std::invalid_argument("SoftBody requires triangles.");
    if (temperature <= 0.0)      throw std::invalid_argument("Temperature must be positive.");

    const size_t N = initialVertices.size();
    p.reserve(N);
    v.assign(N, glm::dvec3(0.0));
    F.assign(N, glm::dvec3(0.0));
    for (const Vertex& vertex : initialVertices) p.emplace_back(vertex.position);

    glm::dvec3 center(0.0);
    for (const glm::dvec3& q : p) center += q;
    center /= static_cast<double>(N);
    for (Triangle& tri : triangles) OrientTriangle(tri, p, center);

    BuildEdges();
    BuildNeighbors();

    indices.reserve(triangles.size() * 3);
    for (const Triangle& tri : triangles) {
        indices.push_back(static_cast<uint32_t>(tri.a));
        indices.push_back(static_cast<uint32_t>(tri.b));
        indices.push_back(static_cast<uint32_t>(tri.c));
    }

    ComputeVolume();
    if (inverted) throw std::runtime_error("SoftBody mesh has non-positive volume.");
    V0 = V;
    Vprev = V0;
    restRadius = std::cbrt(3.0 * V0 / (4.0 * PI));

    UpdateDerived();

    vertices = initialVertices;
    UpdateVertices();
    InitBuffers();

    ComputePressure(0.0);
    std::cout << "Rest volume: " << V0 << " m^3, radius: " << restRadius << " m\n"
              << "Internal pressure: " << P << " Pa (dP = " << dP << ")\n"
              << "Vertex mass: " << vertexMass << " kg, ks = " << params.ks
              << ", kd = " << params.kd << ", bulkDamp = " << bulkDamp << '\n';
}

SoftBody::~SoftBody() {
    if (vertexBufferObject)  glDeleteBuffers(1, &vertexBufferObject);
    if (elementBufferObject) glDeleteBuffers(1, &elementBufferObject);
    if (vertexArrayObject)   glDeleteVertexArrays(1, &vertexArrayObject);
}

SoftBody::SoftBody(SoftBody&& o) noexcept :
    initialVertices(std::move(o.initialVertices)),
    triangles(std::move(o.triangles)),
    indices(std::move(o.indices)),
    edges(std::move(o.edges)),
    neighbors(std::move(o.neighbors)),
    vertices(std::move(o.vertices)),
    p(std::move(o.p)), v(std::move(o.v)), F(std::move(o.F)),
    mix(o.mix), params(o.params),
    T(o.T), Ttarget(o.Ttarget), T0(o.T0),
    V0(o.V0), V(o.V), P(o.P), dP(o.dP), Vprev(o.Vprev),
    bulkDamp(o.bulkDamp), vertexMass(o.vertexMass), restRadius(o.restRadius),
    inverted(o.inverted),
    grabbed(o.grabbed), grabTarget(o.grabTarget),
    vertexArrayObject(std::exchange(o.vertexArrayObject, 0)),
    vertexBufferObject(std::exchange(o.vertexBufferObject, 0)),
    elementBufferObject(std::exchange(o.elementBufferObject, 0))
{}

SoftBody SoftBody::MakeIcosphere(int subdivisions, double radius, glm::dvec3 center, const Mixture& mix, const SoftbodyParams& params, double temperature) {
    if (subdivisions < 0) throw std::invalid_argument("Subdivisions cannot be negative.");
    if (radius <= 0.0)    throw std::invalid_argument("Radius must be positive.");

    std::vector<Vertex> verts;
    std::vector<Triangle> tris;
    BuildIcosphereMesh(subdivisions, radius, center, verts, tris);
    return SoftBody(std::move(verts), std::move(tris), mix, params, temperature);
}

SoftBody SoftBody::MakeEquilibriumIcosphere(int subdivisions, glm::dvec3 center, const Mixture& mix, const SoftbodyParams& params, double temperature) {
    double Veq = EquilibriumVolume(mix, temperature, params.pExt);
    double radius = std::cbrt(Veq / UnitIcosphereVolume(subdivisions));
    return MakeIcosphere(subdivisions, radius, center, mix, params, temperature);
}

void SoftBody::UpdateDerived() {
    const double N = static_cast<double>(p.size());
    const double totalMass = params.skinMassKg + mix.massKg;
    vertexMass = totalMass / N;

    if (params.autoSkin) {
        params.ks = params.targetOverpressure * restRadius / (2.0 * std::sqrt(3.0) * params.maxStrain);
        params.kd = 0.2 * std::sqrt(2.0 * params.ks * vertexMass);
    }

    const double kV = params.pExt / V0;
    const double A = std::cbrt(36.0 * PI * V0 * V0);
    bulkDamp = 2.0 * params.bulkDampingRatio * std::sqrt(kV * totalMass) / A;
}

void SoftBody::InitBuffers() {
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

int SoftBody::RecommendedSubsteps(double dt) const {

    const double totalMass = params.skinMassKg + mix.massKg;
    const double wEdge = std::sqrt(12.0 * params.ks / vertexMass);
    const double wBreath = std::sqrt(12.0 * PI * params.pExt * restRadius / totalMass);

    const double w = std::max(wEdge, wBreath);
    const double hMax = 0.4 / w;
    int n = static_cast<int>(std::ceil(dt / hMax));

    return std::clamp(n, 1, 100);
}

void SoftBody::Step(double dt, int substeps) {
    if (dt <= 0.0 || substeps <= 0) return;
    const double h = dt / static_cast<double>(substeps);

    for (int s = 0; s < substeps; ++s) {
        UpdateTemperature(h);
        ComputeVolume();
        ComputePressure(h);

        ClearForces();
        AddPressureForces();
        AddSpringForces();
        AddVertexForces();

        Integrate(h);
        ResolveCollisions();
    }

    ComputeVolume();
    ComputePressure(0.0);
    UpdateVertices();
}

void SoftBody::UpdateTemperature(double h) {
    if (params.tau <= 0.0) { T = Ttarget; return; }
    T = Ttarget + (T - Ttarget) * std::exp(-h / params.tau);
    T = std::max(T, 1.0);
}

void SoftBody::ComputeVolume() {
    double sum = 0.0;
    for (const Triangle& tri : triangles)
        sum += glm::dot(p[tri.a], glm::cross(p[tri.b], p[tri.c]));

    V = sum / 6.0;
    inverted = V <= MIN_VOLUME;
}

void SoftBody::ComputePressure(double h) {
    if (inverted || mix.n <= 0.0) {
        P = params.pExt;
        dP = 0.0;
        Vprev = V;
        return;
    }

    double R = 8.314;

    const double freeVolume = std::max(V - mix.b_tot, MIN_VOLUME);
    P = mix.n * R * T / freeVolume - mix.a_tot / (V * V);

    double Vdot = 0.0;
    if (h > 0.0) {
        Vdot = (V - Vprev) / h;
        Vprev = V;
    }

    dP = P - params.pExt - bulkDamp * Vdot;
    dP = std::clamp(dP, -params.maxDeltaP, params.maxDeltaP);
}

void SoftBody::ClearForces() {
    std::fill(F.begin(), F.end(), glm::dvec3(0.0));
}

void SoftBody::AddPressureForces() {
    if (inverted) return;
    for (const Triangle& tri : triangles) {
        const glm::dvec3& a = p[tri.a];
        const glm::dvec3& b = p[tri.b];
        const glm::dvec3& c = p[tri.c];

        glm::dvec3 areaNormal = 0.5 * glm::cross(b - a, c - a);
        glm::dvec3 force = (dP / 3.0) * areaNormal;
        F[tri.a] += force;
        F[tri.b] += force;
        F[tri.c] += force;
    }
}

void SoftBody::AddSpringForces() {
    for (const Edge& e : edges) {
        glm::dvec3 d = p[e.j] - p[e.i];
        double L = glm::length(d);
        if (L <= 1e-12) continue;

        glm::dvec3 u = d / L;
        double rv = glm::dot(v[e.j] - v[e.i], u);
        glm::dvec3 f = (params.ks * (L - e.L0) + params.kd * rv) * u;
        F[e.i] += f;
        F[e.j] -= f;
    }
}

void SoftBody::AddVertexForces() {
    const size_t N = p.size();
    const double n = static_cast<double>(N);

    glm::dvec3 vcm(0.0);
    for (const glm::dvec3& vel : v) vcm += vel;
    vcm /= n;

    const double Vpos = std::max(V, 0.0);
    const double rEff = std::cbrt(3.0 * Vpos / (4.0 * PI));
    const double area = PI * rEff * rEff;
    const glm::dvec3 dragCM = -0.5 * params.rhoAir * params.dragCd * area * glm::length(vcm) * vcm / n;

    const glm::dvec3 buoyancy = -params.gravity * (params.rhoAir * Vpos / n);
    const glm::dvec3 weight = vertexMass * params.gravity;

    for (size_t i = 0; i < N; ++i) {
        F[i] += weight + buoyancy + dragCM;
        F[i] -= params.drag * v[i];

        if (params.kb > 0.0 && !neighbors[i].empty()) {
            glm::dvec3 avg(0.0);
            for (int j : neighbors[i]) avg += p[j];
            avg /= static_cast<double>(neighbors[i].size());
            F[i] += params.kb * (avg - p[i]);
        }
    }

    if (grabbed >= 0) F[grabbed] += params.kMouse * (grabTarget - p[grabbed]);
}

void SoftBody::Integrate(double h) {

    for (size_t i = 0; i < p.size(); ++i) {
        v[i] += (F[i] / vertexMass) * h;
        p[i] += v[i] * h;
    }
}

void SoftBody::ResolveCollisions() {
    const double frictionFactor = std::clamp(1.0 - params.friction, 0.0, 1.0);
    for (size_t i = 0; i < p.size(); ++i) {
        if (p[i].y >= params.floorY) continue;
        p[i].y = params.floorY;
        if (v[i].y < 0.0) v[i].y = -v[i].y * params.restitution;
        v[i].x *= frictionFactor;
        v[i].z *= frictionFactor;
    }
}

void SoftBody::BuildEdges() {
    edges.clear();
    std::unordered_set<uint64_t> seen;
    for (const Triangle& tri : triangles) {
        const int idx[3] = {tri.a, tri.b, tri.c};
        for (int k = 0; k < 3; ++k) {
            int i = idx[k], j = idx[(k + 1) % 3];
            if (!seen.insert(EdgeKey(i, j)).second) continue;
            Edge e{};
            e.i = std::min(i, j);
            e.j = std::max(i, j);
            e.L0 = glm::length(p[e.j] - p[e.i]);
            edges.push_back(e);
        }
    }
}

void SoftBody::BuildNeighbors() {
    neighbors.assign(p.size(), {});
    for (const Edge& e : edges) {
        neighbors[e.i].push_back(e.j);
        neighbors[e.j].push_back(e.i);
    }
}

void SoftBody::Reset() {
    for (size_t i = 0; i < p.size(); ++i) p[i] = glm::dvec3(initialVertices[i].position);
    std::fill(v.begin(), v.end(), glm::dvec3(0.0));
    std::fill(F.begin(), F.end(), glm::dvec3(0.0));
    T = Ttarget = T0;
    grabbed = -1;
    ComputeVolume();
    Vprev = V;
    ComputePressure(0.0);
    UpdateVertices();
}

void SoftBody::SetMixture(const Mixture& m) {
    mix = m;
    UpdateDerived();
    ComputePressure(0.0);
}

void SoftBody::Grab(int vertex, glm::dvec3 target) {
    if (vertex < 0 || vertex >= static_cast<int>(p.size())) { grabbed = -1; return; }
    grabbed = vertex;
    grabTarget = target;
}

double SoftBody::MaxStrain() const {
    double worst = 0.0;
    for (const Edge& e : edges) {
        double L = glm::length(p[e.j] - p[e.i]);
        worst = std::max(worst, std::abs(L / e.L0 - 1.0));
    }
    return worst;
}

void SoftBody::UpdateVertices() {
    std::vector<glm::dvec3> normals(p.size(), glm::dvec3(0.0));
    for (const Triangle& tri : triangles) {
        glm::dvec3 n = glm::cross(p[tri.b] - p[tri.a], p[tri.c] - p[tri.a]);   // area-weighted
        normals[tri.a] += n;
        normals[tri.b] += n;
        normals[tri.c] += n;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        vertices[i].position = glm::vec3(p[i]);
        double len = glm::length(normals[i]);
        vertices[i].normal = len > 1e-12 ? glm::vec3(normals[i] / len) : glm::vec3(0.0f, 1.0f, 0.0f);
    }

    if (vertexBufferObject != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    }
}

void SoftBody::Render(Shader& shader) {
    glBindVertexArray(vertexArrayObject);

    shader.SetVector3("color", glm::dvec3(0.925, 0.95, 1.0) * 0.6);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}