#pragma once
#include "vec2.h"
#include "shape.h"
#include <memory>
namespace Physics {
struct RigidBody {
    Vec2 position{0,0};
    float angle{0.0f};
    Vec2 velocity{0,0};
    float angularVelocity{0.0f};
    Vec2 force{0,0};
    float torque{0.0f};
    float mass{1.0f};
    float invMass{1.0f};
    float inertia{1.0f};
    float invInertia{1.0f};
    bool dynamic{true};
    ShapePtr shape{nullptr};
    RigidBody(const Vec2& pos = {0,0}, float m = 1.0f, bool dyn = true)
        : position(pos), mass(m), dynamic(dyn)
    {
        if (!dynamic || mass <= 0.0f) { invMass = 0.0f; mass = 0.0f; }
        else invMass = 1.0f / mass;
        inertia = dynamic ? mass : 0.0f;
        invInertia = inertia > 0.0f ? 1.0f / inertia : 0.0f;
    }
};
using RigidPtr = std::shared_ptr<RigidBody>;
}