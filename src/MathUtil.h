//
// Created by Dmitri on 2026-11-15.
//

#ifndef IDEAL_GAS_LAW_MATHUTIL_H
#define IDEAL_GAS_LAW_MATHUTIL_H
#include <iostream>
#include <stdexcept>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "WorldSettings.h"

struct Mixture {
    double n;
    double a_tot;
    double b_tot;
    double massKg;
};

struct Component {
    GasType type;
    double massFraction;
};

inline double EquilibriumVolume(const Mixture& m, double T, double pExt) {

    double R = 8.314;

    auto f = [&](double V) {
        return m.n * R * T / (V - m.b_tot) - m.a_tot / (V * V) - pExt;
    };

    double guess = m.n * R * T / pExt;
    double lo = std::max(0.5 * guess, m.b_tot * 1.001);
    double hi = 2.0 * guess;

    if (f(lo) < 0.0 || f(hi) > 0.0) {
        throw std::runtime_error("EquilibriumVolume: no gas-branch root bracketed " "(near critical or liquid-like, needs a smarter solver)");
    }
    for (int i = 0; i < 200; ++i) {
        double mid = 0.5 * (lo + hi);
        (f(mid) > 0.0 ? lo : hi) = mid;
    }
    return 0.5 * (lo + hi);
}

inline double molesOf(GasType g, double massKg) {

    float molarMass = calculateMolarMass(gasComposition.at(g));
    return massKg * 1000.0 / molarMass;
}

inline Mixture makeMixture(const std::vector<Component>& mix, double massKg) {

    double sum = 0;
    for (const Component& c : mix) sum += c.massFraction;
    assert(std::abs(sum - 1.0) < 1e-9 && "mass fractions must sum to 1");

    double n = 0, b_tot = 0, a_tot = 0;

    for (const Component& c : mix) {
        double nk = molesOf(c.type, massKg * c.massFraction);
        n += nk;
        b_tot += nk * vanDerWaalsConstants.at(c.type).b / 1000.0;
        a_tot += nk * std::sqrt(vanDerWaalsConstants.at(c.type).a * 0.1);
    }
    return Mixture{ n, a_tot * a_tot, b_tot, massKg };
}

inline glm::mat4 ComputeModelMatrix(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &size) {

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

    glm::mat4 xRotation = glm::rotate(glm::mat4(1.0f), rotation.x * 3.14159265f / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 yRotation = glm::rotate(glm::mat4(1.0f), rotation.y * 3.14159265f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 zRotation = glm::rotate(glm::mat4(1.0f), rotation.z * 3.14159265f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 rot = xRotation * yRotation * zRotation;
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), size);

    return translation * rot * scale;
}

#endif //IDEAL_GAS_LAW_MATHUTIL_H
