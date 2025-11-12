#pragma once
#include "vec2.h"
#include <memory>
namespace Physics {
struct Shape {
    virtual ~Shape() = default;
    virtual Vec2 getSupport(const Vec2& dir) const = 0;
    virtual float getRadius() const = 0;
};
using ShapePtr = std::shared_ptr<Shape>;
}