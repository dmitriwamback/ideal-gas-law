#include <iostream>

#include "src/MathUtil.h"
#include "src/WorldSettings.h"
#include "src/scene/Renderer.h"

int main() {

    Renderer renderer = Renderer();
    renderer.Initialize();
    renderer.GameLoop();
}
