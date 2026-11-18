//
// Created by Dmitri on 2026-11-18.
//

#ifndef IDEAL_GAS_LAW_QUAD_H
#define IDEAL_GAS_LAW_QUAD_H
#include <cstdint>

class Quad {
public:
    Quad() = default;
    void Initialize();
    void Render() const;

private:
    uint32_t vertexArrayObject;
    uint32_t vertexBufferObject;
};

#endif //IDEAL_GAS_LAW_QUAD_H
