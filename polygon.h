#pragma once
#include "shape.h"
#include <vector>
namespace Physics {
struct Polygon : public Shape {
    std::vector<Vec2> verts; // local-space convex polygon (CCW)
    Polygon(const std::vector<Vec2>& v = {}) : verts(v) {}
    Vec2 getSupport(const Vec2& dir) const override {
        float best = -1e30f; Vec2 out{0,0};
        for (auto &p : verts) {
            float d = p.dot(dir);
            if (d > best) { best = d; out = p; }
        }
        return out;
    }
    float getRadius() const override {
        float r = 0;
        for (auto &p : verts) r = std::max(r, p.length());
        return r;
    }
    Vec2 centroid() const {
        if (verts.empty()) return {0,0};
        Vec2 c{0,0}; for (auto &p: verts) c += p;
        return c / static_cast<float>(verts.size());
    }
};
}