#pragma once
#include <cmath>
struct Vec2 {
    float x{0}, y{0};
    constexpr Vec2() = default;
    constexpr Vec2(float x_, float y_): x(x_), y(y_) {}
    constexpr Vec2 operator+(const Vec2& o) const noexcept { return {x+o.x, y+o.y}; }
    constexpr Vec2 operator-(const Vec2& o) const noexcept { return {x-o.x, y-o.y}; }
    constexpr Vec2 operator*(float s) const noexcept { return {x*s, y*s}; }
    constexpr Vec2 operator/(float s) const noexcept { return {x/s, y/s}; }
    Vec2& operator+=(const Vec2& o){ x+=o.x; y+=o.y; return *this; }
    Vec2& operator-=(const Vec2& o){ x-=o.x; y-=o.y; return *this; }
    constexpr float dot(const Vec2& o) const noexcept { return x*o.x + y*o.y; }
    constexpr float cross(const Vec2& o) const noexcept { return x*o.y - y*o.x; }
    float length() const noexcept { return std::sqrt(x*x + y*y); }
    Vec2 normalized() const noexcept { float l = length(); return l>0 ? (*this)/l : Vec2{0,0}; }
};