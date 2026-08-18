#ifndef SYMMETRIC_GROUP_UTILS_H
#define SYMMETRIC_GROUP_UTILS_H
#include "SymmetricGroupElement.h"
#include <vector>
#include <string>
#include <algorithm>

template <int N>
void getConjugacyClassHelper(std::vector<SymmetricGroupElement<N>>& output, std::array<int, N>& partialPermutation, const std::vector<int>& cycleSizes, size_t indexIntoCycleSizes, std::array<bool, N>& alreadyIncludedInPermutation);

// TODO: avoid repeating when multiple of same cycle size
template <int N>
void addAllCyclesToPartialHelper(std::vector<SymmetricGroupElement<N>>& output, std::array<int, N>& partialPermutation, const std::vector<int>& cycleSizes, size_t indexIntoCycleSizes, std::array<bool, N>& alreadyIncludedInPermutation, int cycleProgress, int prevCycleElement, int firstCycleElement) {
    int cycleSize = cycleSizes[indexIntoCycleSizes];
    if (cycleProgress == cycleSize) {
        // we've finished adding a cycle, so now we recurse onto the next cycles.
        getConjugacyClassHelper<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes + 1, alreadyIncludedInPermutation);
    } else {
        for (int i = firstCycleElement; i < N; ++i) { // start from firstCycleElement to ensure we only count each cycle once
            if (!alreadyIncludedInPermutation[i]) {
                alreadyIncludedInPermutation[i] = true;
                if (cycleProgress == 0) {
                    // If cycleProgress is zero, then we do not update partialPermutation.
                    // At this stage, prevCycleElement has no value, so we don't have the required information to update it.
                    addAllCyclesToPartialHelper<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes, alreadyIncludedInPermutation, 1, i, i);
                } else {
                    partialPermutation[prevCycleElement] = i;
                    if (cycleProgress == cycleSize - 1) {
                        partialPermutation[i] = firstCycleElement;
                    }
                    addAllCyclesToPartialHelper<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes, alreadyIncludedInPermutation, cycleProgress + 1, i, firstCycleElement);
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

template <int N>
void addAllCyclesToPartial(std::vector<SymmetricGroupElement<N>>& output, std::array<int, N>& partialPermutation, const std::vector<int>& cycleSizes, size_t indexIntoCycleSizes, std::array<bool, N>& alreadyIncludedInPermutation) {
    addAllCyclesToPartialHelper<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes, alreadyIncludedInPermutation, 0, -1, 0);
}

template <int N>
void getConjugacyClassHelper(std::vector<SymmetricGroupElement<N>>& output, std::array<int, N>& partialPermutation, const std::vector<int>& cycleSizes, size_t indexIntoCycleSizes, std::array<bool, N>& alreadyIncludedInPermutation) {
    if (indexIntoCycleSizes < cycleSizes.size()) {
        addAllCyclesToPartial<N>(output, partialPermutation, cycleSizes, indexIntoCycleSizes, alreadyIncludedInPermutation);
    } else {
        // make a copy of the final partialPermutation (now a complete permutation) and move it into a SymmetricGroupElement.
        output.push_back(SymmetricGroupElement<N>(std::array(partialPermutation)));
    }
}

template <int N>
std::vector<SymmetricGroupElement<N>> getConjugacyClass(const SymmetricGroupElement<N>& el) { // conjugacy class in symmetric group
    std::vector<SymmetricGroupElement<N>> output;
    //TODO: consider using a bitset instead, Or just uint16_t
    std::array<bool, N> alreadyIncludedInCycle;
    alreadyIncludedInCycle.fill(false);
    std::array<int, N> partialPermutation;
    // set up partial permutation to fix each point by default
    for (int i = 0; i < N; ++i) {
        partialPermutation[i] = i;
    }
    std::vector<int> cycleStructure = el.getCycleStructure();
    std::sort(cycleStructure.begin(), cycleStructure.end(), std::greater<int>()); //sort descending;
    getConjugacyClassHelper<N>(output, partialPermutation, cycleStructure, 0, alreadyIncludedInCycle);
    return output;
}


template <int N>
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

#endif