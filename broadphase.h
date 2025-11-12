#pragma once
#include "rigidbody.h"
#include <vector>
namespace Physics {
struct BroadPhase {
    float cellSize{1.0f};
    BroadPhase(float cs=1.0f): cellSize(cs) {}
    std::vector<std::pair<size_t,size_t>> potentialPairs(const std::vector<RigidPtr>& bodies);
};
}