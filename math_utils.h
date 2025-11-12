#pragma once
#include "vec2.h"
#include <algorithm>
inline float clampf(float v, float a, float b) { return std::max(a, std::min(b, v)); }
inline Vec2 perpendicular(const Vec2& v) { return {-v.y, v.x}; }