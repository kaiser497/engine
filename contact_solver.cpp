#include "contact_solver.h"
#include "math_utils.h"
#include <algorithm>
namespace Physics {
void applyImpulse(Contact& c) {
    auto A = c.a; auto B = c.b;
    Vec2 ra = c.contactPoint - A->position;
    Vec2 rb = c.contactPoint - B->position;
    Vec2 rv = (B->velocity + perpendicular(rb) * B->angularVelocity) - (A->velocity + perpendicular(ra) * A->angularVelocity);
    float velAlongNormal = rv.dot(c.normal);
    if (velAlongNormal > 0) return;
    float e = std::min(A ? c.restitution : 0.0f, B ? c.restitution : 0.0f);
    float invMassSum = A->invMass + B->invMass + (ra.cross(c.normal)*ra.cross(c.normal))*A->invInertia + (rb.cross(c.normal)*rb.cross(c.normal))*B->invInertia;
    if (invMassSum == 0.0f) return;
    float j = -(1.0f + e) * velAlongNormal / invMassSum;
    Vec2 impulse = c.normal * j;
    if (A->dynamic) { A->velocity -= impulse * A->invMass; A->angularVelocity -= ra.cross(impulse) * A->invInertia; }
    if (B->dynamic) { B->velocity += impulse * B->invMass; B->angularVelocity += rb.cross(impulse) * B->invInertia; }
    rv = (B->velocity + perpendicular(rb) * B->angularVelocity) - (A->velocity + perpendicular(ra) * A->angularVelocity);
    Vec2 tangent = rv - c.normal * rv.dot(c.normal);
    float tlen = tangent.length();
    tangent = tlen ? tangent / tlen : Vec2{0,0};
    float jt = -rv.dot(tangent) / invMassSum;
    float mu = std::sqrt(c.a->shape->getRadius() * c.b->shape->getRadius()) * c.friction;
    Vec2 frictionImpulse;
    if (std::fabs(jt) < j * mu) frictionImpulse = tangent * jt;
    else frictionImpulse = tangent * (-j * mu);
    if (A->dynamic) { A->velocity -= frictionImpulse * A->invMass; A->angularVelocity -= ra.cross(frictionImpulse) * A->invInertia; }
    if (B->dynamic) { B->velocity += frictionImpulse * B->invMass; B->angularVelocity += rb.cross(frictionImpulse) * B->invInertia; }
}
void positionalCorrection(Contact& c) {
    const float percent = 0.8f; const float slop = 0.01f;
    float invMassSum = c.a->invMass + c.b->invMass;
    if (invMassSum == 0.0f) return;
    Vec2 correction = c.normal * (std::max(c.penetration - slop, 0.0f) / invMassSum) * percent;
    if (c.a->dynamic) c.a->position -= correction * c.a->invMass;
    if (c.b->dynamic) c.b->position += correction * c.b->invMass;
}
void ContactSolver::resolve(std::vector<Contact>& contacts, float dt) {
    if (contacts.empty()) return;
    const int iterations = 10;
    for (int i=0;i<iterations;++i) {
        for (auto &c : contacts) applyImpulse(c);
    }
    for (auto &c : contacts) positionalCorrection(c);
}
}