#include "engine.h"
#include "collision.h"
#include "broadphase.h"
#include "contact_solver.h"
#include <memory>
#include <algorithm>
namespace Physics {
Engine::Engine() = default;
Engine::~Engine() = default;
RigidPtr Engine::createRigidBody(const Vec2& pos, float mass, bool dynamic) {
    auto b = std::make_shared<RigidBody>(pos, mass, dynamic);
    bodies.push_back(b);
    return b;
}
void Engine::integrateForces(RigidPtr b, float dt) {
    if (!b->dynamic) return;
    b->force += gravity * b->mass;
    Vec2 acc = b->force * b->invMass;
    b->velocity += acc * dt;
    b->angularVelocity += b->torque * b->invInertia * dt;
    b->force = {0,0};
    b->torque = 0.0f;
}
void Engine::integrateVelocities(RigidPtr b, float dt) {
    if (!b->dynamic) return;
    b->position += b->velocity * dt;
    b->angle += b->angularVelocity * dt;
    b->velocity *= 0.999f;
    b->angularVelocity *= 0.98f;
}
void Engine::step(float dt) {
    for (auto &b : bodies) integrateForces(b, dt);
    auto pairs = broad.potentialPairs(bodies);
    contacts.clear();
    for (auto &p : pairs) {
        Contact c; c.a = bodies[p.first]; c.b = bodies[p.second];
        if (!c.a->shape || !c.b->shape) continue;
        if (dynamic_cast<Circle*>(c.a->shape.get()) && dynamic_cast<Circle*>(c.b->shape.get())) {
            if (circle_vs_circle(c)) contacts.push_back(c);
        } else if (dynamic_cast<Circle*>(c.a->shape.get()) && dynamic_cast<Polygon*>(c.b->shape.get())) {
            if (circle_vs_polygon(c)) contacts.push_back(c);
        } else if (dynamic_cast<Polygon*>(c.a->shape.get()) && dynamic_cast<Circle*>(c.b->shape.get())) {
            std::swap(c.a,c.b);
            if (circle_vs_polygon(c)) { c.normal = c.normal * -1.0f; contacts.push_back(c); }
        } else {
            if (polygon_vs_polygon(c)) contacts.push_back(c);
        }
    }
    solver.resolve(contacts, dt);
    for (auto &b : bodies) integrateVelocities(b, dt);
}
}