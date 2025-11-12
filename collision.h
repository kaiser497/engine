#pragma once
#include "rigidbody.h"
#include <vector>
namespace Physics {
struct Contact {
    RigidPtr a;
    RigidPtr b;
    Vec2 normal;
    float penetration{0.0f};
    float restitution{0.2f};
    float friction{0.2f};
    Vec2 contactPoint;
};
bool circle_vs_circle(Contact& c);
bool circle_vs_polygon(Contact& c);
bool polygon_vs_polygon(Contact& c);
std::vector<Contact> generateContacts(const std::vector<RigidPtr>& bodies);
}