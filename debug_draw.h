#pragma once
#include "rigidbody.h"
#include <iostream>
inline void debug_draw_body(const Physics::RigidPtr& b) {
    std::cout << "[DBG] pos=(" << b->position.x << "," << b->position.y << ") ang=" << b->angle
              << " vel=(" << b->velocity.x << "," << b->velocity.y << ")\n";
}