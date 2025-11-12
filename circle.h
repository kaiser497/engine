#pragma once
#include "shape.h"
namespace Physics {
struct Circle : public Shape {
    float radius{0.5f};
    Circle(float r=0.5f): radius(r) {}
    Vec2 getSupport(const Vec2& dir) const override {
        float l = dir.length();
        return l > 0 ? dir / l * radius : Vec2{radius, 0};
    }
    float getRadius() const override { return radius; }
};
}