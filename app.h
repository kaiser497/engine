#pragma once
#include "engine.h"
#include "renderer.h"
#include <SFML/Graphics.hpp>
class App {
public:
    App();
    int run();
private:
    Physics::Engine engine;
    Renderer renderer;
    bool paused{false};
    void setupScene();
};