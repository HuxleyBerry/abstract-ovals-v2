#ifndef ORDERLY_WITHOUT_PROPERTY_H
#define ORDERLY_WITHOUT_PROPERTY_H

//TODO: Remove this file. I'm pretty sure the idea I had here does not work.

#include <vector>
#include "MinimalImageUtils.h"
#include "../SymmetricGroupElement.h"

//TODO: can I remove this?
template <size_t N>
struct StabilizerData {
    std::vector<SymmetricGroupElement<N>> stabilizer;
    bool isFresh = false;
};


template <size_t N>
bool shouldWeRecurse(const std::vector<SymmetricGroupElement<N>>& universe, std::vector<size_t>& partialSet, size_t selectedIndex, std::vector<std::vector<SymmetricGroupElement<N>>>& stabiliserCache) {
    size_t workingIndex = partialSet.size();
    SymmetricGroupElement<N> minInOrbitFromActionOfWholeGroup = getMinimalImageOfInvolution(universe[selectedIndex]);
    if (workingIndex == 0) {
        return universe[selectedIndex] == minInOrbitFromActionOfWholeGroup;
    }
    if (minInOrbitFromActionOfWholeGroup < partialSet[0]) {
        return false;
    }
    for (size_t i = 0; i <= workingIndex - 2; ++i) {
        if (stabiliserCache.size() <= i) {
            if (i == 0) {
                stabiliserCache.push_back(getInvolutionStabilizer(universe[partialSet[0]]));
            } else {
                stabiliserCache.push_back(getInvolutionStabilizerGivenGroup(universe[partialSet[i]], stabiliserCache[i - 1]));
            }
        }
        for (const SymmetricGroupElement<N>& stabilizerElement : stabiliserCache[i]) {
            if (conjugate(universe[selectedIndex], stabilizerElement) < partialSet[i + 1]) {
                return false;
            }
        }
        return true;
    }
}

template <size_t N>
void attemptOrderlyExtension(const std::vector<SymmetricGroupElement<N>>& universe, std::vector<size_t>& partialSet, std::vector<std::vector<SymmetricGroupElement<N>>>& stabiliserCache, size_t minimumSelectableIndex, size_t maximumSize, std::vector<std::vector<SymmetricGroupElement<N>>>& results) {
    if (partialSet.size() == maximumSize) {
        results.push_back(partialSet);
        return;
    }
    for (size_t idx = minimumSelectableIndex; idx < universe.size() - (maximumSize - partialSet.size() - 1); ++idx) {
        // we now check if appending universe[idx] to partialSet would result in something lexicographically minimal.
        if (shouldWeRecurse(universe, partialSet, idx, stabiliserCache)) {
            // TODO: consider avoiding copying
            std::vector<size_t> newPartialSet = partialSet;
            newPartialSet.push_back(idx);
            attemptOrderlyExtension(universe, newPartialSet, stabiliserCache, idx + 1, maximumSize, results);
        }
    }
}

// assumes universe is ordered.
// Finds all subsets of universe of size maximumSize that are lexicographically ordered.
template <size_t N>
std::vector<std::vector<SymmetricGroupElement<N>>> performOrderlyAlgorithm(const std::vector<SymmetricGroupElement<N>>& universe, size_t maximumSize) {
    std::vector<std::vector<SymmetricGroupElement<N>>> stabiliserCache(0);
    std::vector<size_t> initialPartialSet(0);
    std::vector<std::vector<SymmetricGroupElement<N>>> results;
    attemptOrderlyExtension(universe, initialPartialSet, stabiliserCache, 0, maximumSize, results);
    return results;
}

#endif