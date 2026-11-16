#include <iostream>

#include "src/WorldSettings.h"

int main() {

    float demoMassKg = 1.0f;
    float weightsHelium = 0.65f;
    float weightsNitrousOxide = 1.0f - weightsHelium;

    float heliumMolarMassKg = calculateMolarMass(gasComposition.at(GasType::HeliumGas)) / 1000.0f;
    float nitrousOxideMolarMassKg = calculateMolarMass(gasComposition.at(GasType::NitrousOxide)) / 1000.0f;

    float molsHelium = demoMassKg * weightsHelium / heliumMolarMassKg;
    float molsNitrousOxide = demoMassKg * weightsNitrousOxide / nitrousOxideMolarMassKg;

    std::cout << "Helium: " << heliumMolarMassKg * 1000.0f << "g/mol" << std::endl;
    std::cout << "Nitrous Oxide: " << nitrousOxideMolarMassKg * 1000.0f << "g/mol" << std::endl;

    std::cout << "Demo mass: " << demoMassKg << "kg" << std::endl;
    std::cout << "mols Helium: " << molsHelium << "mol" << std::endl;
    std::cout << "mols Nitrous Oxide: " << molsNitrousOxide << "mol" << std::endl;
}