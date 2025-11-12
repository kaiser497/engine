#include "broadphase.h"
#include <unordered_map>
#include <cmath>
namespace Physics {
static inline long long packKey(int x, int y) {
    return ( (long long)(x) << 32 ) | (unsigned int)y;
}
std::vector<std::pair<size_t,size_t>> BroadPhase::potentialPairs(const std::vector<RigidPtr>& bodies) {
    std::unordered_map<long long, std::vector<size_t>> grid;
    for (size_t i=0;i<bodies.size();++i) {
        auto &b = bodies[i];
        int gx = static_cast<int>(std::floor(b->position.x / cellSize));
        int gy = static_cast<int>(std::floor(b->position.y / cellSize));
        long long key = packKey(gx,gy);
        grid[key].push_back(i);
    }
    std::vector<std::pair<size_t,size_t>> pairs;
    for (auto &cell : grid) {
        auto &vec = cell.second;
        for (size_t i=0;i<vec.size();++i) for (size_t j=i+1;j<vec.size();++j) pairs.emplace_back(vec[i], vec[j]);
        long long k = cell.first;
        int gx = (int)(k >> 32);
        int gy = (int)(k & 0xffffffff);
        for (int ox=-1; ox<=1; ++ox) for (int oy=-1; oy<=1; ++oy) {
            long long nk = packKey(gx+ox, gy+oy);
            if (nk==k) continue;
            auto it = grid.find(nk);
            if (it==grid.end()) continue;
            for (auto a : vec) for (auto b : it->second) {
                if (a < b) pairs.emplace_back(a,b);
                else pairs.emplace_back(b,a);
            }
        }
    }
    std::sort(pairs.begin(), pairs.end());
    pairs.erase(std::unique(pairs.begin(), pairs.end()), pairs.end());
    return pairs;
}
}