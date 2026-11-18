//
// Created by Dmitri on 2026-08-22.
//

#ifndef IDEAL_GAS_LAW_WORLDSETTINGS_H
#define IDEAL_GAS_LAW_WORLDSETTINGS_H
#include <map>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct WorldSettings {
    float ground = 0.0f;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct Triangle {
    int a, b, c;
};

struct Edge {
    int i, j;
    double L0;
};

struct SoftbodyParams {
    bool autoSkin = true;
    double targetOverpressure = 500.0;
    double maxStrain = 0.02;
    double ks = 0.0, kd = 0.0;
    double kb = 0.0;

    double skinMassKg = 0.05;
    double pExt = 101325.0;
    double rhoAir = 1.2;
    glm::dvec3 gravity{0.0, -9.81, 0.0};
    double maxDeltaP = 5e4;

    double drag = 0.0;
    double dragCd = 1.0;
    double bulkDampingRatio = 0.3;

    double restitution = 0.1, friction = 0.3;
    double tau = 1.5;
    double kMouse = 50.0;
    double floorY = -10.0;
};

struct Element {
    int atomicNumber;
    float molarMass;
};

struct Gas {
    double a;
    double b;
};

enum class GasType {
    HydrogenGas,            HeliumGas,              NitrogenGas,
    OxygenGas,              CarbonDioxide,          WaterVapor,
    Methane,                Ammonia,                SulfurDioxide,
    ChlorineGas,            NeonGas,                ArgonGas,
    KryptonGas,             XenonGas,               RadonGas,
    CarbonMonoxide,         NitrousOxide,           HydrogenSulfide,
    Phosgene,               Ethylene,               Acetylene,
    Propane,                Butane,                 Pentane,
    Hexane,                 Heptane,                Octane,
    UraniumHexafluoride,    TungstenHexafluoride,   Ozone
};

enum class ElementType {
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

inline const std::map<ElementType, Element> elements = {
    {ElementType::Hydrogen,    {1, 1.008f}},     {ElementType::Helium,      {2, 4.002f}},     {ElementType::Lithium,     {3, 6.941f}},     {ElementType::Beryllium,   {4, 9.012f}},
    {ElementType::Boron,       {5, 10.81f}},     {ElementType::Carbon,      {6, 12.01f}},     {ElementType::Nitrogen,    {7, 14.01f}},     {ElementType::Oxygen,      {8, 16.00f}},
    {ElementType::Fluorine,    {9, 19.00f}},     {ElementType::Neon,        {10, 20.18f}},    {ElementType::Sodium,      {11, 22.99f}},    {ElementType::Magnesium,   {12, 24.31f}},
    {ElementType::Aluminum,    {13, 26.98f}},    {ElementType::Silicon,     {14, 28.09f}},    {ElementType::Phosphorus,  {15, 30.97f}},    {ElementType::Sulfur,      {16, 32.06f}},
    {ElementType::Chlorine,    {17, 35.45f}},    {ElementType::Argon,       {18, 39.95f}},    {ElementType::Potassium,   {19, 39.10f}},    {ElementType::Calcium,     {20, 40.08f}},
    {ElementType::Scandium,    {21, 44.96f}},    {ElementType::Titanium,    {22, 47.87f}},    {ElementType::Vanadium,    {23, 50.94f}},    {ElementType::Chromium,    {24, 52.00f}},
    {ElementType::Manganese,   {25, 54.94f}},    {ElementType::Iron,        {26, 55.85f}},    {ElementType::Cobalt,      {27, 58.93f}},    {ElementType::Nickel,      {28, 58.69f}},
    {ElementType::Copper,      {29, 63.55f}},    {ElementType::Zinc,        {30, 65.38f}},    {ElementType::Gallium,     {31, 69.72f}},    {ElementType::Germanium,   {32, 72.63f}},
    {ElementType::Arsenic,     {33, 74.92f}},    {ElementType::Selenium,    {34, 78.96f}},    {ElementType::Bromine,     {35, 79.90f}},    {ElementType::Krypton,     {36, 83.80f}},
    {ElementType::Rubidium,    {37, 85.47f}},    {ElementType::Strontium,   {38, 87.62f}},    {ElementType::Yttrium,     {39, 88.91f}},    {ElementType::Zirconium,   {40, 91.22f}},
    {ElementType::Niobium,     {41, 92.91f}},    {ElementType::Molybdenum,  {42, 95.95f}},    {ElementType::Technetium,  {43, 98.00f}},    {ElementType::Ruthenium,   {44, 101.07f}},
    {ElementType::Rhodium,     {45, 102.91f}},   {ElementType::Palladium,   {46, 106.42f}},   {ElementType::Silver,      {47, 107.87f}},   {ElementType::Cadmium,     {48, 112.41f}},
    {ElementType::Indium,      {49, 114.82f}},   {ElementType::Tin,         {50, 118.71f}},   {ElementType::Antimony,    {51, 121.76f}},   {ElementType::Tellurium,   {52, 127.60f}},
    {ElementType::Iodine,      {53, 126.90f}},   {ElementType::Xenon,       {54, 131.29f}},   {ElementType::Cesium,      {55, 132.91f}},   {ElementType::Barium,      {56, 137.33f}},
    {ElementType::Lanthanum,   {57, 138.91f}},   {ElementType::Cerium,      {58, 140.12f}},   {ElementType::Praseodymium,{59, 140.91f}},   {ElementType::Neodymium,   {60, 144.24f}},
    {ElementType::Promethium,  {61, 145.00f}},   {ElementType::Samarium,    {62, 150.36f}},   {ElementType::Europium,    {63, 151.96f}},   {ElementType::Gadolinium,  {64, 157.25f}},
    {ElementType::Terbium,     {65, 158.93f}},   {ElementType::Dysprosium,  {66, 162.50f}},   {ElementType::Holmium,     {67, 164.93f}},   {ElementType::Erbium,      {68, 167.26f}},
    {ElementType::Thulium,     {69, 168.93f}},   {ElementType::Ytterbium,   {70, 173.05f}},   {ElementType::Lutetium,    {71, 174.97f}},   {ElementType::Hafnium,     {72, 178.49f}},
    {ElementType::Tantalum,    {73, 180.95f}},   {ElementType::Tungsten,    {74, 183.84f}},   {ElementType::Rhenium,     {75, 186.21f}},   {ElementType::Osmium,      {76, 190.23f}},
    {ElementType::Iridium,     {77, 192.22f}},   {ElementType::Platinum,    {78, 195.08f}},   {ElementType::Gold,        {79, 196.97f}},   {ElementType::Mercury,     {80, 200.59f}},
    {ElementType::Thallium,    {81, 204.38f}},   {ElementType::Lead,        {82, 207.2f}},    {ElementType::Bismuth,     {83, 208.98f}},   {ElementType::Polonium,    {84, 209.00f}},
    {ElementType::Astatine,    {85, 210.00f}},   {ElementType::Radon,       {86, 222.00f}},   {ElementType::Francium,    {87, 223.00f}},   {ElementType::Radium,      {88, 226.00f}},
    {ElementType::Actinium,    {89, 227.00f}},   {ElementType::Thorium,     {90, 232.04f}},   {ElementType::Protactinium,{91, 231.04f}},   {ElementType::Uranium,     {92, 238.03f}},
    {ElementType::Neptunium,   {93, 237.00f}},   {ElementType::Plutonium,   {94, 244.00f}},   {ElementType::Americium,   {95, 243.00f}},   {ElementType::Curium,      {96, 247.00f}},
    {ElementType::Berkelium,   {97, 247.00f}},   {ElementType::Californium, {98, 251.00f}},   {ElementType::Einsteinium, {99, 252.00f}},   {ElementType::Fermium,     {100, 257.00f}},
    {ElementType::Mendelevium, {101, 258.00f}},  {ElementType::Nobelium,    {102, 259.00f}},  {ElementType::Lawrencium,  {103, 262.00f}},  {ElementType::Rutherfordium,{104, 267.00f}},
    {ElementType::Dubnium,     {105, 270.00f}},  {ElementType::Seaborgium,  {106, 271.00f}},  {ElementType::Bohrium,     {107, 270.00f}},  {ElementType::Hassium,     {108, 277.00f}},
    {ElementType::Meitnerium,  {109, 276.00f}},  {ElementType::Darmstadtium,{110, 281.00f}},  {ElementType::Roentgenium, {111, 282.00f}},  {ElementType::Copernicium, {112, 285.00f}},
    {ElementType::Nihonium,    {113, 286.00f}},  {ElementType::Flerovium,   {114, 289.00f}},  {ElementType::Moscovium,   {115, 288.00f}},  {ElementType::Livermorium, {116, 293.00f}},
    {ElementType::Tennessine,  {117, 294.00f}},  {ElementType::Oganesson,   {118, 294.00f}}
};

inline const std::map<GasType, Gas> vanDerWaalsConstants = {
    {GasType::HydrogenGas,          {0.2476f, 0.02661f}},
    {GasType::HeliumGas,            {0.0346f, 0.0238f}},
    {GasType::NitrogenGas,          {1.370f, 0.0387f}},
    {GasType::OxygenGas,            {1.382f, 0.03186f}},
    {GasType::CarbonDioxide,        {3.640f, 0.04267f}},
    {GasType::WaterVapor,           {5.536f, 0.03049f}},
    {GasType::Methane,              {2.283f, 0.04278f}},
    {GasType::Ammonia,              {4.225f, 0.0371f}},
    {GasType::SulfurDioxide,        {6.803f, 0.05636f}},
    {GasType::ChlorineGas,          {6.579f, 0.05622f}},
    {GasType::NeonGas,              {0.2135f, 0.01709f}},
    {GasType::ArgonGas,             {1.355f, 0.03201f}},
    {GasType::KryptonGas,           {2.349f, 0.03978f}},
    {GasType::XenonGas,             {4.250f, 0.05105f}},
    {GasType::RadonGas,             {6.601f, 0.06239f}},
    {GasType::CarbonMonoxide,       {1.505f, 0.03985f}},
    {GasType::Ozone,                {3.545f, 0.0487f}},
    {GasType::NitrousOxide,         {3.832f, 0.04415f}},
    {GasType::HydrogenSulfide,      {4.544f, 0.04339f}},
    {GasType::Phosgene,             {9.171f, 0.0831f}},
    {GasType::Ethylene,             {4.612f, 0.0582f}},
    {GasType::Acetylene,            {4.516f, 0.0522f}},
    {GasType::Propane,              {8.779f, 0.08445f}},
    {GasType::TungstenHexafluoride, {13.25f, 0.1063f}},
    {GasType::Butane,               {14.66f, 0.1226f}},
    {GasType::UraniumHexafluoride,  {16.01f, 0.1128f}},
    {GasType::Pentane,              {19.26f, 0.146f}},
    {GasType::Hexane,               {24.71f, 0.1735f}},
    {GasType::Heptane,              {31.06f, 0.2049f}},
    {GasType::Octane,               {37.88f, 0.2374f}},
};


// Helps to compute the molar mass and the number of moles in a given substance of mass m
inline const std::map<GasType, std::map<ElementType, int>> gasComposition = {
    {GasType::HydrogenGas,          {{ElementType::Hydrogen, 2}}},
    {GasType::HeliumGas,            {{ElementType::Helium,   1}}},
    {GasType::NitrogenGas,          {{ElementType::Nitrogen, 2}}},
    {GasType::OxygenGas,            {{ElementType::Oxygen,   2}}},
    {GasType::CarbonDioxide,        {{ElementType::Carbon,   1}, {ElementType::Oxygen, 2}}},
    {GasType::WaterVapor,           {{ElementType::Hydrogen, 2}, {ElementType::Oxygen, 1}}},
    {GasType::Methane,              {{ElementType::Carbon,   1}, {ElementType::Hydrogen, 4}}},
    {GasType::Ammonia,              {{ElementType::Nitrogen, 1}, {ElementType::Hydrogen, 3}}},
    {GasType::SulfurDioxide,        {{ElementType::Sulfur,   1}, {ElementType::Oxygen, 2}}},
    {GasType::ChlorineGas,          {{ElementType::Chlorine, 2}}},
    {GasType::NeonGas,              {{ElementType::Neon,     1}}},
    {GasType::ArgonGas,             {{ElementType::Argon,    1}}},
    {GasType::KryptonGas,           {{ElementType::Krypton,  1}}},
    {GasType::XenonGas,             {{ElementType::Xenon,    1}}},
    {GasType::RadonGas,             {{ElementType::Radon,    1}}},
    {GasType::CarbonMonoxide,       {{ElementType::Carbon,   1}, {ElementType::Oxygen, 1}}},
    {GasType::Ozone,                {{ElementType::Oxygen, 3}}},
    {GasType::NitrousOxide,         {{ElementType::Nitrogen, 2}, {ElementType::Oxygen, 1}}},
    {GasType::HydrogenSulfide,      {{ElementType::Hydrogen, 2}, {ElementType::Sulfur, 1}}},
    {GasType::Phosgene,             {{ElementType::Carbon,   1}, {ElementType::Oxygen, 1}, {ElementType::Chlorine, 2}}},
    {GasType::Ethylene,             {{ElementType::Carbon,   2}, {ElementType::Hydrogen, 4}}},
    {GasType::Acetylene,            {{ElementType::Carbon,   2}, {ElementType::Hydrogen, 2}}},
    {GasType::Propane,              {{ElementType::Carbon,   3}, {ElementType::Hydrogen, 8}}},
    {GasType::TungstenHexafluoride, {{ElementType::Tungsten, 1}, {ElementType::Fluorine, 6}}},
    {GasType::Butane,               {{ElementType::Carbon,   4}, {ElementType::Hydrogen, 10}}},
    {GasType::UraniumHexafluoride,  {{ElementType::Uranium,  1}, {ElementType::Fluorine, 6}}},
    {GasType::Pentane,              {{ElementType::Carbon,   5}, {ElementType::Hydrogen, 12}}},
    {GasType::Hexane,               {{ElementType::Carbon,   6}, {ElementType::Hydrogen, 14}}},
    {GasType::Heptane,              {{ElementType::Carbon,   7}, {ElementType::Hydrogen, 16}}},
    {GasType::Octane,               {{ElementType::Carbon,   8}, {ElementType::Hydrogen, 18}}},
};

inline float calculateMolarMass(const std::map<ElementType, int>& _elements) {

    float total = 0;
    for (const auto& [element, count] : _elements) {
        total += elements.at(element).molarMass * count;
    }
    return total;
}

#endif