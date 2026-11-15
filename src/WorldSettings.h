//
// Created by Dmitri on 2026-08-22.
//

#ifndef IDEAL_GAS_LAW_WORLDSETTINGS_H
#define IDEAL_GAS_LAW_WORLDSETTINGS_H
#include <map>
#include <glm/vec2.hpp>

struct WorldSettings {
    glm::vec2 gravity = glm::vec2(0.0f, -980.0f);
    float ground = 0.0f;
};

struct Element {
    int atomicNumber;
    float molarMass;
};

enum GasType {
    HydrogenGas,        HeliumGas,      NitrogenGas,
    OxygenGas,          CarbonDioxide,  WaterVapor,
    Methane,            Ammonia,        SulfurDioxide,
    ChlorineGas,        NeonGas,        ArgonGas,
    KryptonGas,         XenonGas,       RadonGas,
    CarbonMonoxide,     NitrousOxide,   HydrogenSulfide,
    Phosgene,           Ethylene,       Acetylene,
    Propane,            Butane,         Pentane,
    Hexane,             Heptane,        Octane
};

enum ElementType {
    Hydrogen,       Helium,         Lithium,        Beryllium,
    Boron,          Carbon,         Nitrogen,       Oxygen,
    Fluorine,       Neon,           Sodium,         Magnesium,
    Aluminum,       Silicon,        Phosphorus,     Sulfur,
    Chlorine,       Argon,          Potassium,      Calcium,
    Scandium,       Titanium,       Vanadium,       Chromium,
    Manganese,      Iron,           Cobalt,         Nickel,
    Copper,         Zinc,           Gallium,        Germanium,
    Arsenic,        Selenium,       Bromine,        Krypton,
    Rubidium,       Strontium,      Yttrium,        Zirconium,
    Niobium,        Molybdenum,     Technetium,     Ruthenium,
    Rhodium,        Palladium,      Silver,         Cadmium,
    Indium,         Tin,            Antimony,       Tellurium,
    Iodine,         Xenon,          Cesium,         Barium,
    Lanthanum,      Cerium,         Praseodymium,   Neodymium,
    Promethium,     Samarium,       Europium,       Gadolinium,
    Terbium,        Dysprosium,     Holmium,        Erbium,
    Thulium,        Ytterbium,      Lutetium,       Hafnium,
    Tantalum,       Tungsten,       Rhenium,        Osmium,
    Iridium,        Platinum,       Gold,           Mercury,
    Thallium,       Lead,           Bismuth,        Polonium,
    Astatine,       Radon,          Francium,       Radium,
    Actinium,       Thorium,        Protactinium,   Uranium,
    Neptunium,      Plutonium,      Americium,      Curium,
    Berkelium,      Californium,    Einsteinium,    Fermium,
    Mendelevium,    Nobelium,       Lawrencium,     Rutherfordium,
    Dubnium,        Seaborgium,     Bohrium,        Hassium,
    Meitnerium,     Darmstadtium,   Roentgenium,    Copernicium,
    Nihonium,       Flerovium,      Moscovium,      Livermorium,
    Tennessine,     Oganesson
};

struct Gas {
    float a;
    float b;
};

inline std::map<ElementType, Element> elements = {
    {Hydrogen,    {1, 1.008f}},     {Helium,      {2, 4.002f}},     {Lithium,     {3, 6.941f}},     {Beryllium,   {4, 9.012f}},
    {Boron,       {5, 10.81f}},     {Carbon,      {6, 12.01f}},     {Nitrogen,    {7, 14.01f}},     {Oxygen,      {8, 16.00f}},
    {Fluorine,    {9, 19.00f}},     {Neon,        {10, 20.18f}},    {Sodium,      {11, 22.99f}},    {Magnesium,   {12, 24.31f}},
    {Aluminum,    {13, 26.98f}},    {Silicon,     {14, 28.09f}},    {Phosphorus,  {15, 30.97f}},    {Sulfur,      {16, 32.06f}},
    {Chlorine,    {17, 35.45f}},    {Argon,       {18, 39.95f}},    {Potassium,   {19, 39.10f}},    {Calcium,     {20, 40.08f}},
    {Scandium,    {21, 44.96f}},    {Titanium,    {22, 47.87f}},    {Vanadium,    {23, 50.94f}},    {Chromium,    {24, 52.00f}},
    {Manganese,   {25, 54.94f}},    {Iron,        {26, 55.85f}},    {Cobalt,      {27, 58.93f}},    {Nickel,      {28, 58.69f}},
    {Copper,      {29, 63.55f}},    {Zinc,        {30, 65.38f}},    {Gallium,     {31, 69.72f}},    {Germanium,   {32, 72.63f}},
    {Arsenic,     {33, 74.92f}},    {Selenium,    {34, 78.96f}},    {Bromine,     {35, 79.90f}},    {Krypton,     {36, 83.80f}},
    {Rubidium,    {37, 85.47f}},    {Strontium,   {38, 87.62f}},    {Yttrium,     {39, 88.91f}},    {Zirconium,   {40, 91.22f}},
    {Niobium,     {41, 92.91f}},    {Molybdenum,  {42, 95.95f}},    {Technetium,  {43, 98.00f}},    {Ruthenium,   {44, 101.07f}},
    {Rhodium,     {45, 102.91f}},   {Palladium,   {46, 106.42f}},   {Silver,      {47, 107.87f}},   {Cadmium,     {48, 112.41f}},
    {Indium,      {49, 114.82f}},   {Tin,         {50, 118.71f}},   {Antimony,    {51, 121.76f}},   {Tellurium,   {52, 127.60f}},
    {Iodine,      {53, 126.90f}},   {Xenon,       {54, 131.29f}},   {Cesium,      {55, 132.91f}},   {Barium,      {56, 137.33f}},
    {Lanthanum,   {57, 138.91f}},   {Cerium,      {58, 140.12f}},   {Praseodymium,{59, 140.91f}},   {Neodymium,   {60, 144.24f}},
    {Promethium,  {61, 145.00f}},   {Samarium,    {62, 150.36f}},   {Europium,    {63, 151.96f}},   {Gadolinium,  {64, 157.25f}},
    {Terbium,     {65, 158.93f}},   {Dysprosium,  {66, 162.50f}},   {Holmium,     {67, 164.93f}},   {Erbium,      {68, 167.26f}},
    {Thulium,     {69, 168.93f}},   {Ytterbium,   {70, 173.05f}},   {Lutetium,    {71, 174.97f}},   {Hafnium,     {72, 178.49f}},
    {Tantalum,    {73, 180.95f}},   {Tungsten,    {74, 183.84f}},   {Rhenium,     {75, 186.21f}},   {Osmium,      {76, 190.23f}},
    {Iridium,     {77, 192.22f}},   {Platinum,    {78, 195.08f}},   {Gold,        {79, 196.97f}},   {Mercury,     {80, 200.59f}},
    {Thallium,    {81, 204.38f}},   {Lead,        {82, 207.2f}},    {Bismuth,     {83, 208.98f}},   {Polonium,    {84, 209.00f}},
    {Astatine,    {85, 210.00f}},   {Radon,       {86, 222.00f}},   {Francium,    {87, 223.00f}},   {Radium,      {88, 226.00f}},
    {Actinium,    {89, 227.00f}},   {Thorium,     {90, 232.04f}},   {Protactinium,{91, 231.04f}},   {Uranium,     {92, 238.03f}},
    {Neptunium,   {93, 237.00f}},   {Plutonium,   {94, 244.00f}},   {Americium,   {95, 243.00f}},   {Curium,      {96, 247.00f}},
    {Berkelium,   {97, 247.00f}},   {Californium, {98, 251.00f}},   {Einsteinium, {99, 252.00f}},   {Fermium,     {100, 257.00f}},
    {Mendelevium, {101, 258.00f}},  {Nobelium,    {102, 259.00f}},  {Lawrencium,  {103, 262.00f}},  {Rutherfordium,{104, 267.00f}},
    {Dubnium,     {105, 270.00f}},  {Seaborgium,  {106, 271.00f}},  {Bohrium,     {107, 270.00f}},  {Hassium,     {108, 277.00f}},
    {Meitnerium,  {109, 276.00f}},  {Darmstadtium,{110, 281.00f}},  {Roentgenium, {111, 282.00f}},  {Copernicium, {112, 285.00f}},
    {Nihonium,    {113, 286.00f}},  {Flerovium,   {114, 289.00f}},  {Moscovium,   {115, 288.00f}},  {Livermorium, {116, 293.00f}},
    {Tennessine,  {117, 294.00f}},  {Oganesson,   {118, 294.00f}}
};

inline std::map<GasType, Gas> gasAttractiveForces = {
    {GasType::HydrogenGas, {0.2476f, 0.02661f}},
    {GasType::HeliumGas, {0.0346f, 0.0238f}},
    {GasType::NitrogenGas, {1.370f, 0.0387f}},
    {GasType::OxygenGas, {1.382f, 0.03186f}},
    {GasType::CarbonDioxide, {3.640f, 0.04267f}},
    {GasType::WaterVapor, {5.536f, 0.03049f}},
    {GasType::Methane, {2.283f, 0.04278f}},
    {GasType::Ammonia, {4.225f, 0.0371f}},
    {GasType::SulfurDioxide, {6.803f, 0.05636f}},
    {GasType::ChlorineGas, {6.579f, 0.05622f}},
    {GasType::NeonGas, {0.2135f, 0.01709f}},
    {GasType::ArgonGas, {1.355f, 0.03201f}},
    {GasType::KryptonGas, {2.349f, 0.03978f}},
    {GasType::XenonGas, {4.250f, 0.05105f}},
    {GasType::RadonGas, {6.601f, 0.06239f}},
    {GasType::CarbonMonoxide, {1.505f, 0.03985f}},
    {GasType::NitrousOxide, {3.832f, 0.04415f}},
    {GasType::HydrogenSulfide, {4.544f, 0.04339f}},
    {GasType::Phosgene, {9.171f, 0.0831f}},
    {GasType::Ethylene, {4.612f, 0.0582f}},
    {GasType::Acetylene, {4.516f, 0.0522f}},
    {GasType::Propane, {8.779f, 0.08445f}},
    {GasType::Butane, {14.66f, 0.1226f}},
    {GasType::Pentane, {19.26f, 0.146f}},
    {GasType::Hexane, {24.71f, 0.1735f}},
    {GasType::Heptane, {31.06f, 0.2049f}},
    {GasType::Octane, {37.88f, 0.2374f}}
};


// Helps to compute the molar mass and the number of moles in a given substance of mass m
inline std::map<GasType, std::vector<std::map<ElementType, int>>> gasComposition = {
    {GasType::HydrogenGas,      {{{Hydrogen, 2}}}},
    {GasType::HeliumGas,        {{{Helium,   1}}}},
    {GasType::NitrogenGas,      {{{Nitrogen, 2}}}},
    {GasType::OxygenGas,        {{{Oxygen,   2}}}},
    {GasType::CarbonDioxide,    {{{Carbon,   1}, {Oxygen, 2}}}},
    {GasType::WaterVapor,       {{{Hydrogen, 2}, {Oxygen, 1}}}},
    {GasType::Methane,          {{{Carbon,   1}, {Hydrogen, 4}}}},
    {GasType::Ammonia,          {{{Nitrogen, 1}, {Hydrogen, 3}}}},
    {GasType::SulfurDioxide,    {{{Sulfur,   1}, {Oxygen, 1}}}},
    {GasType::ChlorineGas,      {{{Chlorine, 2}}}},
    {GasType::NeonGas,          {{{Neon,     1}}}},
    {GasType::ArgonGas,         {{{Argon,    1}}}},
    {GasType::KryptonGas,       {{{Krypton,  1}}}},
    {GasType::XenonGas,         {{{Xenon,    1}}}},
    {GasType::RadonGas,         {{{Radon,    1}}}},
    {GasType::CarbonMonoxide,   {{{Carbon,   1}, {Oxygen, 1}}}},
    {GasType::NitrousOxide,     {{{Nitrogen, 2}, {Oxygen, 1}}}},
    {GasType::HydrogenSulfide,  {{{Hydrogen, 2}, {Sulfur, 1}}}},
    {GasType::Phosgene,         {{{Carbon,   1}, {Oxygen, 1}, {Chlorine, 2}}}},
    {GasType::Ethylene,         {{{Carbon,   2}, {Hydrogen, 4}}}},
    {GasType::Acetylene,        {{{Carbon,   2}, {Hydrogen, 2}}}},
    {GasType::Propane,          {{{Carbon,   3}, {Hydrogen, 8}}}},
    {GasType::Butane,           {{{Carbon,   4}, {Hydrogen, 10}}}},
    {GasType::Pentane,          {{{Carbon,   5}, {Hydrogen, 12}}}},
    {GasType::Hexane,           {{{Carbon,   6}, {Hydrogen, 14}}}},
    {GasType::Heptane,          {{{Carbon,   7}, {Hydrogen, 16}}}},
    {GasType::Octane,           {{{Carbon,   8}, {Hydrogen, 18}}}},
};

inline float calculateMolarMass(std::vector<std::map<ElementType, int>> _elements) {

    float total = 0;
    for (const auto& composition : _elements) {
        for (const auto& [element, count] : composition) {
            total += elements.at(element).molarMass * count;
        }
    }
    return total;
}

#endif