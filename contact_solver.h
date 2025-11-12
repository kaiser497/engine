#pragma once
#include "collision.h"
namespace Physics {
struct ContactSolver {
    void resolve(std::vector<Contact>& contacts, float dt);
};
}