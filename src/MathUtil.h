//
// Created by Dmitri on 2026-11-15.
//

#ifndef IDEAL_GAS_LAW_MATHUTIL_H
#define IDEAL_GAS_LAW_MATHUTIL_H
#include <iostream>

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
#endif //IDEAL_GAS_LAW_MATHUTIL_H
