#pragma once
#include "vec2.h"
#include <cmath>
struct Transform {
    Vec2 position{0,0};
    float rotation{0}; // radians
    Transform() = default;
    Transform(const Vec2& p, float r): position(p), rotation(r) {}
    Vec2 apply(const Vec2& local) const {
        float c = std::cos(rotation), s = std::sin(rotation);
        return { position.x + local.x*c - local.y*s,
                 position.y + local.x*s + local.y*c };
    }
};