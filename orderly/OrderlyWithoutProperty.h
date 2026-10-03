#ifndef ORDERLY_WITHOUT_PROPERTY_H
#define ORDERLY_WITHOUT_PROPERTY_H

#include <vector>
#include "../SymmetricGroupElement.h"


template <size_t N>
bool attemptOrderlyExtension(const std::vector<SymmetricGroupElement<N>>& universe, std::vector<size_t>& partialSet, std::vector<std::vector<SymmetricGroupElement<N>>>& stabiliserCache, size_t minimumSelectableIndex, size_t maximumSize) {
    size_t workingIndex = partialSet.size();
    for (size_t idx = minimumSelectableIndex; idx < universe.size() - (maximumSize - partialSet.size() - 1); ++idx) {
        // we now check if appending universe[idx] to partialSet would result in something lexicographically minimal.

        for (size_t i = 0; i < workingIndex; ++i) {

        }
    }
}

// assumes universe is ordered.
// Finds all subsets of universe of size maximumSize that are lexicographically ordered.
template <size_t N>
bool performOrderlyAlgorithm(const std::vector<SymmetricGroupElement<N>>& universe, size_t maximumSize) {

}

#endif