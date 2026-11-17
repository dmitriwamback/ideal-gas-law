#include <iostream>

#include "src/MathUtil.h"
#include "src/WorldSettings.h"

int main() {

    std::vector gases = {
        GasType::HeliumGas,
        GasType::HydrogenGas
    };

    std::vector weights = {
        0.95,
        0.05
    };

    std::vector<Component> components = {};

    for (int i = 0; i < gases.size(); i++) {
        GasType gas = gases[i];
        double weight = weights[i];

        Component component{};
        component.massFraction = weight;
        component.type = gas;

        components.push_back(component);
    }

    Mixture m = makeMixture(components, 1);

    std::cout << "a total: " << m.a_tot << std::endl;
    std::cout << "b total: " << m.b_tot << std::endl;
    std::cout << "moles: " << m.n << std::endl;
}