#include "MinimalImageUtils.h"
#include <vector>
#include <cstdint>
#include <utility>

void getAllPermutationsHelper(size_t permSize, int currentPoint, std::uint16_t doneBitSet, const std::vector<size_t>& currentPerm, std::vector<std::vector<size_t>>& outVector) {
    if (currentPoint == permSize) {
        outVector.push_back(currentPerm);
    } else {
        for (size_t i = 0; i < permSize; ++i) {
            if (((doneBitSet >> i) & 1) == 0) {
                std::vector<size_t> permCopy(currentPerm);
                permCopy[currentPoint] = i;
                std::uint16_t newBitSet = doneBitSet | (1 << i);
                getAllPermutationsHelper(permSize, currentPoint + 1, newBitSet, permCopy, outVector);
            }
        }
    }
}

std::vector<std::vector<size_t>> getAllPermutations(int permSize) {
    std::vector<std::vector<size_t>> allPerms;
    std::vector<size_t> currentPerm(permSize);
    std::uint16_t doneBitSet = 0;
    getAllPermutationsHelper(permSize, 0, doneBitSet, currentPerm, allPerms);
    return allPerms;
}