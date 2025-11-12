#include "app.h"
#include <memory>
#include "circle.h"
#include "polygon.h"
App::App(): renderer(1920,1080) {
    setupScene();
}
void App::setupScene() {
    auto g = engine.createRigidBody({0.0f, -6.0f}, 0.0f, false);
    g->shape = std::make_shared<Physics::Polygon>(std::vector<Physics::Vec2>{{-50,-1},{50,-1},{50,1},{-50,1}});
    for (int i=0;i<10;++i) {
        for (int j=0;j<3;++j) {
            auto b = engine.createRigidBody({-6.0f + i*1.2f, 2.0f + j*1.0f}, 1.0f, true);
            if ((i+j)%2==0) b->shape = std::make_shared<Physics::Circle>(0.4f + (j*0.05f));
            else b->shape = std::make_shared<Physics::Polygon>(std::vector<Physics::Vec2>{{-0.4f,-0.4f},{0.4f,-0.4f},{0.4f,0.4f},{-0.4f,0.4f}});
        }
    }
}
int App::run() {
    sf::RenderWindow window(sf::VideoMode(1920,1080), "EngineX - Simulation", sf::Style::Close);
    window.setFramerateLimit(120);
    const float dt = 1.0f/120.0f;
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
            if (ev.type == sf::Event::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Space) paused = !paused;
                if (ev.key.code == sf::Keyboard::Escape) window.close();
            }
            if (ev.type == sf::Event::MouseButtonPressed) {
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    float mx = (ev.mouseButton.x - 960.0f)/50.0f;
                    float my = (540.0f - ev.mouseButton.y)/50.0f;
                    auto b = engine.createRigidBody({mx, my}, 1.0f, true);
                    b->shape = std::make_shared<Physics::Circle>(0.35f);
                    b->velocity = { (float)(std::rand()%10 - 5), (float)(std::rand()%5 + 2) };
                }
            }
        }
        if (!paused) {
            engine.step(dt);
        }
        renderer.render(window, engine.bodies);
    }
    return 0;
}