#include "collision.h"
#include "circle.h"
#include "polygon.h"
#include "transform.h"
#include "math_utils.h"
#include <algorithm>
#include <limits>
namespace Physics {
bool circle_vs_circle(Contact& c) {
    auto A = c.a; auto B = c.b;
    Vec2 n = B->position - A->position;
    float r = A->shape->getRadius() + B->shape->getRadius();
    float dist2 = n.x*n.x + n.y*n.y;
    if (dist2 > r*r) return false;
    float dist = std::sqrt(dist2);
    if (dist == 0.0f) { c.penetration = A->shape->getRadius(); c.normal = {1,0}; c.contactPoint = A->position; }
    else { c.penetration = r - dist; c.normal = n / dist; c.contactPoint = A->position + c.normal * (A->shape->getRadius() - c.penetration*0.5f); }
    c.restitution = 0.3f; c.friction = 0.4f;
    return true;
}
static void projectPolygon(const Polygon& poly, const Vec2& axis, const Transform& t, float& min, float& max) {
    bool first = true;
    for (auto &v : poly.verts) {
        Vec2 world = t.apply(v);
        float p = world.dot(axis);
        if (first) { min = max = p; first = false; }
        else { min = std::min(min,p); max = std::max(max,p); }
    }
}
static bool polygon_vs_polygon_SAT(const Polygon& A, const Polygon& B, const Transform& ta, const Transform& tb, Vec2& outNormal, float& outPen) {
    float bestPen = std::numeric_limits<float>::infinity();
    Vec2 bestNormal{0,0};
    auto checkAxes = [&](const std::vector<Vec2>& verts, const Transform& t) {
        for (size_t i=0;i<verts.size();++i) {
            Vec2 va = t.apply(verts[i]);
            Vec2 vb = t.apply(verts[(i+1)%verts.size()]);
            Vec2 edge = vb - va;
            Vec2 axis = perpendicular(edge).normalized();
            float amin,amax,bmin,bmax;
            projectPolygon(A, axis, ta, amin, amax);
            projectPolygon(B, axis, tb, bmin, bmax);
            if (amax < bmin || bmax < amin) return false;
            float pen = std::min(amax - bmin, bmax - amin);
            if (pen < bestPen) { bestPen = pen; bestNormal = axis; }
        }
        return true;
    };
    if (!checkAxes(A.verts, ta)) return false;
    if (!checkAxes(B.verts, tb)) return false;
    outNormal = bestNormal; outPen = bestPen;
    return true;
}
bool polygon_vs_polygon(Contact& c) {
    auto A = std::dynamic_pointer_cast<Polygon>(c.a->shape);
    auto B = std::dynamic_pointer_cast<Polygon>(c.b->shape);
    if (!A || !B) return false;
    Transform ta{c.a->position, c.a->angle}, tb{c.b->position, c.b->angle};
    Vec2 normal; float pen;
    if (!polygon_vs_polygon_SAT(*A,*B,ta,tb,normal,pen)) return false;
    c.normal = normal; c.penetration = pen; c.contactPoint = (c.a->position + c.b->position) * 0.5f;
    c.restitution = 0.2f; c.friction = 0.5f;
    return true;
}
bool circle_vs_polygon(Contact& c) {
    auto C = std::dynamic_pointer_cast<Circle>(c.a->shape);
    auto P = std::dynamic_pointer_cast<Polygon>(c.b->shape);
    if (!C || !P) return false;
    Transform tp{c.b->position, c.b->angle};
    float bestDist = std::numeric_limits<float>::infinity();
    Vec2 bestPoint{0,0}, bestNormal{0,0};
    for (size_t i=0;i<P->verts.size();++i) {
        Vec2 v0 = tp.apply(P->verts[i]);
        Vec2 v1 = tp.apply(P->verts[(i+1)%P->verts.size()]);
        Vec2 edge = v1 - v0;
        float t = clampf((c.a->position - v0).dot(edge) / edge.dot(edge), 0.0f, 1.0f);
        Vec2 proj = v0 + edge * t;
        Vec2 diff = c.a->position - proj;
        float d = diff.length();
        if (d < bestDist) { bestDist = d; bestPoint = proj; bestNormal = diff.length() ? diff / d : Vec2{1,0}; }
    }
    float r = C->radius;
    if (bestDist > r) return false;
    c.penetration = r - bestDist;
    c.normal = bestNormal;
    c.contactPoint = bestPoint;
    c.restitution = 0.3f; c.friction = 0.4f;
    return true;
}
std::vector<Contact> generateContacts(const std::vector<RigidPtr>& bodies) {
    std::vector<Contact> contacts;
    for (size_t i=0;i<bodies.size();++i) for (size_t j=i+1;j<bodies.size();++j) {
        Contact c; c.a = bodies[i]; c.b = bodies[j];
        if (!c.a->dynamic && !c.b->dynamic) continue;
        float rsum = c.a->shape->getRadius() + c.b->shape->getRadius();
        Vec2 diff = c.b->position - c.a->position;
        if (diff.length() > rsum + 2.0f) continue;
        bool collided = false;
        if (dynamic_cast<Circle*>(c.a->shape.get()) && dynamic_cast<Circle*>(c.b->shape.get())) {
            collided = circle_vs_circle(c);
        } else if (dynamic_cast<Circle*>(c.a->shape.get()) && dynamic_cast<Polygon*>(c.b->shape.get())) {
            collided = circle_vs_polygon(c);
        } else if (dynamic_cast<Polygon*>(c.a->shape.get()) && dynamic_cast<Circle*>(c.b->shape.get())) {
            std::swap(c.a,c.b);
            collided = circle_vs_polygon(c);
            if (collided) { c.normal = c.normal * -1.0f; }
        } else {
            collided = polygon_vs_polygon(c);
        }
        if (collided) contacts.push_back(c);
    }
    return contacts;
}
}