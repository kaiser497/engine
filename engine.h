#pragma once
#include "rigidbody.h"
#include <vector>
#include "broadphase.h"
#include "contact_solver.h"
namespace Physics {
class Engine {
public:
    Engine();
    ~Engine();
    RigidPtr createRigidBody(const Vec2& pos, float mass, bool dynamic=true);
    void step(float dt);
    std::vector<RigidPtr> bodies;
    Vec2 gravity{0.0f, -9.81f};
private:
    BroadPhase broad{1.0f};
    ContactSolver solver;
    std::vector<Contact> contacts;
    void integrateForces(RigidPtr b, float dt);
    void integrateVelocities(RigidPtr b, float dt);
};
}