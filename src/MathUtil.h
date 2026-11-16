//
// Created by Dmitri on 2026-11-15.
//

#ifndef IDEAL_GAS_LAW_MATHUTIL_H
#define IDEAL_GAS_LAW_MATHUTIL_H
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

double molesOf(GasType g, double massKg) {

    float molarMass = calculateMolarMass(gasComposition.at(g));
    return massKg / (molarMass * 1000.0);
}

Mixture makeMixture(const std::vector<Component>& mix, double massKg) {

    Mixture m{};

    for (Component& component : mix) {
        double mass = massKg * component.massFraction;
        double moles = molesOf(component.type, mass);


    }
}

#endif //IDEAL_GAS_LAW_MATHUTIL_H
