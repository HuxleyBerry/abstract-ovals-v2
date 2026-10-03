#ifndef SYMMETRIC_GROUP_UTILS_H
#define SYMMETRIC_GROUP_UTILS_H
#include "SymmetricGroupElement.h"
#include <vector>
#include <string>
#include <algorithm>

namespace {

    template <size_t N>
    void getConjugacyClassHelper(std::vector<SymmetricGroupElement<N>>& output, std::array<size_t, N>& partialPermutation, const std::vector<int>& cycleSizes, size_t indexIntoCycleSizes, std::array<bool, N>& alreadyIncludedInPermutation, int firstPointInMostRecentCycle);

    template <size_t N>
    void addAllCyclesToPartialHelper(std::vector<SymmetricGroupElement<N>>& output, std::array<size_t, N>& partialPermutation, const std::vector<int>& cycleSizes, size_t indexIntoCycleSizes, std::array<bool, N>& alreadyIncludedInPermutation, int cycleProgress, int prevCycleElement, int firstCycleElement, int firstPointInMostRecentCycle) {
        int cycleSize = cycleSizes[indexIntoCycleSizes];
        if (cycleProgress == cycleSize) {
            // we've finished adding a cycle, so now we recurse onto the next cycles.
            getConjugacyClassHelper<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes + 1, alreadyIncludedInPermutation, firstCycleElement);
        } else {
            int start;
            if (cycleProgress == 0) {
                bool isCycleSameSizeAsPrevCycle = indexIntoCycleSizes != 0 && cycleSizes[indexIntoCycleSizes] == cycleSizes[indexIntoCycleSizes - 1];
                start = isCycleSameSizeAsPrevCycle ? firstPointInMostRecentCycle + 1 : 0;
            } else {
                start = firstCycleElement + 1;
            }
            
            for (int i = start; i < N; ++i) { // start from firstCycleElement to ensure we only count each cycle once
                if (!alreadyIncludedInPermutation[i]) {
                    alreadyIncludedInPermutation[i] = true;
                    if (cycleProgress == 0) {
                        // If cycleProgress is zero, then we do not update partialPermutation.
                        // At this stage, prevCycleElement has no value, so we don't have the required information to update it.
                        addAllCyclesToPartialHelper<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes, alreadyIncludedInPermutation, 1, i, i, -1);
                    } else {
                        partialPermutation[prevCycleElement] = i;
                        if (cycleProgress == cycleSize - 1) {
                            partialPermutation[i] = firstCycleElement;
                        }
                        addAllCyclesToPartialHelper<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes, alreadyIncludedInPermutation, cycleProgress + 1, i, firstCycleElement, -1);
                        //backtrack
                        partialPermutation[prevCycleElement] = prevCycleElement;
                        if (cycleProgress == cycleSize - 1) {
                            partialPermutation[i] = i;
                        }
                    }
                    //backtrack
                    alreadyIncludedInPermutation[i] = false;
                }
            }
        }
    }

    template <size_t N>
    void getConjugacyClassHelper(std::vector<SymmetricGroupElement<N>>& output, std::array<size_t, N>& partialPermutation, const std::vector<int>& cycleSizes, size_t indexIntoCycleSizes, std::array<bool, N>& alreadyIncludedInPermutation, int firstPointInMostRecentCycle) {
        if (indexIntoCycleSizes < cycleSizes.size()) {
            addAllCyclesToPartialHelper<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes, alreadyIncludedInPermutation, 0, -1, 0, firstPointInMostRecentCycle);
        } else {
            output.push_back(SymmetricGroupElement<N>(partialPermutation));
        }
    }
}

template <size_t N>
std::vector<SymmetricGroupElement<N>> getConjugacyClass(const SymmetricGroupElement<N>& el) { // conjugacy class in symmetric group
    std::vector<SymmetricGroupElement<N>> output;
    // TODO: consider using a bitset instead, Or just uint16_t
    std::array<bool, N> alreadyIncludedInCycle;
    alreadyIncludedInCycle.fill(false);
    std::array<size_t, N> partialPermutation;
    // set up partial permutation to fix each point by default
    for (int i = 0; i < N; ++i) {
        partialPermutation[i] = i;
    }
    std::vector<int> cycleStructure = el.getCycleStructure();
    std::sort(cycleStructure.begin(), cycleStructure.end(), std::greater<int>()); //sort descending;
    getConjugacyClassHelper<N>(output, partialPermutation, cycleStructure, 0, alreadyIncludedInCycle, -1);
    return output;
}


template <size_t N>
bool operator<(const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right) {
    const std::array<size_t, N>& leftArray = left.getInternalArray();
    const std::array<size_t, N>& rightArray = right.getInternalArray();
    for (int i = 0; i < N; ++i) {
        if (leftArray[i] < rightArray[i]) {
            return true;
        } else if (leftArray[i] > rightArray[i]) {
            return false;
        }
    }
    return false;
}

template <size_t N>
bool operator<=(const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right) {
    const std::array<size_t, N>& leftArray = left.getInternalArray();
    const std::array<size_t, N>& rightArray = right.getInternalArray();
    for (int i = 0; i < N; ++i) {
        if (leftArray[i] != rightArray[i]) {
            return false;
        }
    }
    return true;
}

template <size_t N>
bool operator>(const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right) {
    return !(left <= right);
}

template <size_t N>
bool operator>=(const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right) {
    return !(left < right);
}

template <size_t N>
std::string listOfPermutationsToString(const std::vector<SymmetricGroupElement<N>>& list) {
    std::string ans = "{ ";
    for (int i = 0; i < list.size(); ++i) {
        ans += list[i].toString();
        if (i != list.size() - 1) {
            ans += ", ";
        }
    }
    ans += " }";
    return ans;
}

template <size_t N>
std::vector<SymmetricGroupElement<N>> conjugatePermutationSet(const std::vector<SymmetricGroupElement<N>>& set, const SymmetricGroupElement<N>& conjugateBy) {
    std::vector<SymmetricGroupElement<N>> result;
    result.reserve(set.size());
    for (const auto& el: set) {
        result.push_back(conjugate(el, conjugateBy));
    }
    return result;
}

template <size_t N>
bool isPermutationSetLexicographicallyLess(const std::vector<SymmetricGroupElement<N>>& left, const std::vector<SymmetricGroupElement<N>>& right) {
    for (size_t i = 0; i < std::min(left.size(), right.size()); ++i) {
        if (left[i] < right[i]) {
            return true;
        } else if (left[i] > right[i]) {
            return false;
        }
    }
    return left.size() < right.size();
}

#endif