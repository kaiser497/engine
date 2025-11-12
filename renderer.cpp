#include "renderer.h"
#include "circle.h"
#include "polygon.h"
#include <cmath>
Renderer::Renderer(int w, int h) {
    view = sf::View(sf::FloatRect(0,0,(float)w,(float)h));
    if (!font.loadFromFile("arial.ttf")) {
        // ok if missing
    }
}
static sf::Color toColor(int i) {
    static sf::Color cols[] = { sf::Color(200,200,220), sf::Color(120,180,200), sf::Color(140,200,140), sf::Color(220,200,120), sf::Color(200,140,200), sf::Color(200,140,140) };
    return cols[i % 6];
}
void Renderer::render(sf::RenderWindow& win, const std::vector<Physics::RigidPtr>& bodies) {
    win.clear(sf::Color(20,24,30));
    int idx=0;
    for (auto &b : bodies) {
        if (!b->shape) continue;
        if (auto c = dynamic_cast<Physics::Circle*>(b->shape.get())) {
            sf::CircleShape s(c->radius * 50.0f);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition(b->position.x * 50.0f + 960.0f, 540.0f - b->position.y * 50.0f);
            s.setFillColor(toColor(idx));
            win.draw(s);
        } else if (auto p = dynamic_cast<Physics::Polygon*>(b->shape.get())) {
            sf::ConvexShape poly;
            size_t n = p->verts.size();
            poly.setPointCount(n);
            for (size_t i=0;i<n;++i) {
                auto v = p->verts[i];
                float x = v.x * 50.0f + 960.0f;
                float y = 540.0f - v.y * 50.0f;
                poly.setPoint(i, sf::Vector2f(x,y));
            }
            poly.setFillColor(toColor(idx));
            win.draw(poly);
        }
        ++idx;
    }
    win.display();
}