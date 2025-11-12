#pragma once
#include "rigidbody.h"
#include <SFML/Graphics.hpp>
#include <memory>
namespace Physics { class Engine; }
class Renderer {
public:
    Renderer(int w=1920, int h=1080);
    void render(sf::RenderWindow& win, const std::vector<Physics::RigidPtr>& bodies);
    sf::View view;
private:
    sf::Font font;
};