#ifndef MINIMAL_IMAGE_H
#define MINIMAL_IMAGE_H

#include <vector>
#include <array>
#include <utility>
#include <cstdint>
#include <optional>
#include <algorithm>
#include "../SymmetricGroupElement.h"
#include "MinimalImageUtils.h"

namespace {
    enum class MinimalStatus {
        NotMinimal,
        Undetermined
    };
}

// If indexIntoPermutation is i, it means we are evaluating a candidate for the ith coordinate at this poit in the recursive process
// If indexIntoPermutation is i, and the involution set is A = {a_0,...,a_(N-1)}, then minimumAchievingGroupElement is *a* an element g of 
// S_N such that {a_0,..a_i}^g is minimal.
template <size_t N>
MinimalStatus isSetOfInvolutionsMinimalHelper(const std::vector<SymmetricGroupElement<N>>& involutions, std::vector<bool>& isIncludedInPermutation, std::vector<size_t>& partialPermutation, int indexIntoPermutation, std::vector<std::vector<SymmetricGroupElement<N>>>& pointwiseStabilisers, const SymmetricGroupElement<N>& minimumAchievingGroupElement) {
    SymmetricGroupElement<N> newMinimumAchievingGroupElement("()");
    if (indexIntoPermutation != -1) {
        SymmetricGroupElement<N> minimumInOrbit;
        if (indexIntoPermutation == 0) {
            std::tie(minimumInOrbit, newMinimumAchievingGroupElement) = getMinimalImageOfInvolutionWithGroupElement(involutions[partialPermutation[indexIntoPermutation]]);
        } else {
            if (pointwiseStabilisers.size() <= indexIntoPermutation - 1) {
                if (indexIntoPermutation == 1) {
                    pointwiseStabilisers.push_back(getInvolutionStabilizer(involutions[0]));
                } else {
                    pointwiseStabilisers.push_back(getInvolutionStabilizerGivenGroup(involutions[indexIntoPermutation - 1], pointwiseStabilisers[indexIntoPermutation - 2]));
                }
            }
            bool flag = true;
            SymmetricGroupElement<N> bestGroupElement;
            for (const SymmetricGroupElement<N>& stabilizerElement : pointwiseStabilisers[indexIntoPermutation - 1]) {
                const SymmetricGroupElement<N> rightCosetMember = stabilizerElement * minimumAchievingGroupElement;
                const SymmetricGroupElement<N> image = conjugate(involutions[partialPermutation[indexIntoPermutation]], rightCosetMember); if (flag || image < minimumInOrbit) {
                    minimumInOrbit = image;
                    bestGroupElement = rightCosetMember;
                }
                flag = false;
            }
            newMinimumAchievingGroupElement = bestGroupElement;
        }
        if (minimumInOrbit < involutions[indexIntoPermutation]) {
            return MinimalStatus::NotMinimal;
        } else if (minimumInOrbit > involutions[indexIntoPermutation]) {
            return MinimalStatus::Undetermined;
        }
    }
    for (size_t i = 0; i < involutions.size(); ++i) {
        if (!isIncludedInPermutation[i]) {
            isIncludedInPermutation[i] = true;
            partialPermutation[indexIntoPermutation + 1] = i;
            MinimalStatus status = isSetOfInvolutionsMinimalHelper(involutions, isIncludedInPermutation, partialPermutation, indexIntoPermutation + 1, pointwiseStabilisers, newMinimumAchievingGroupElement);
            if (status == MinimalStatus::NotMinimal) {
                return MinimalStatus::NotMinimal;
            }
            isIncludedInPermutation[i] = false;
        }
    }
    return MinimalStatus::Undetermined;
}

// Assumes the vector is sorted.
// Returns true if the set of involutions is minimal (lexicographically) in its orbit with respect
// to the conjugation action of the whole of S_N.
template <size_t N>
bool isSetOfInvolutionsMinimal(const std::vector<SymmetricGroupElement<N>>& involutions) {
    std::vector<std::vector<SymmetricGroupElement<N>>> pointwiseStabilizers;
    std::vector<bool> isIncludedInPermutation(involutions.size(), false);
    std::vector<size_t> partialPermutation(involutions.size());
    return isSetOfInvolutionsMinimalHelper(involutions, isIncludedInPermutation, partialPermutation, -1, pointwiseStabilizers, SymmetricGroupElement<N>("()")) != MinimalStatus::NotMinimal;
}

template <size_t N>
bool isUnsortedSetOfInvolutionsMinimal(std::vector<SymmetricGroupElement<N>> involutions) {
    std::sort(involutions.begin(), involutions.end());
    return isSetOfInvolutionsMinimal<N>(involutions);
}

#endif