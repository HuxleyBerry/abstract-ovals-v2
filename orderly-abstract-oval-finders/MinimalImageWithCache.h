#ifndef MINIMAL_IMAGE_CACHE_H
#define MINIMAL_IMAGE_CACHE_H

#include <vector>
#include <array>
#include <utility>
#include <cstdint>
#include <optional>
#include <algorithm>
#include "../SymmetricGroupElement.h"
#include "../orderly/MinimalImageUtils.h"

namespace MinimalImageWithCache {
    enum class MinimalStatus {
        NotMinimal,
        Undetermined
    };

    template <size_t N>
    struct StabilizerCache {
        std::vector<std::vector<SymmetricGroupElement<N>>> pointwiseStabilizers; // index i holds the pointwise stabilizer of involutions 0..i
        int cacheFreshUpTo; // the largest index of pointwiseStabilizers that the cache is fresh at

        StabilizerCache(): pointwiseStabilizers(std::vector<std::vector<SymmetricGroupElement<N>>>((N - 1) * (N - 1))), cacheFreshUpTo(-1) {}
    };

    /*
    Parameters:
    - allInvolutions: allows access to a symmetric group element from an index
    - involutionSelectionIndices: the indices within allInvolutions of the involutions we are trying to check for minimality. Will be in increasing order
    - selectionSize: the index of involutionSelectionIndices beyond which we ignore. We keep junk data beyond this point in involutionSelectionIndices as
      a performance improvement (don't need to create a new vector)
    - partialPermutation: the permutation of the indices up to selectionSize which is currently being considered at this stage in the recursive process.
    - isIncludedInPermutation: A vector of bools indicating whether a given index is already included in the partial permutation
    - indexIntoPermutation: the largest index of the partial permutation we have already set at this point in the recursive progress
    - sCache: The nth element of this vector holds the pointwise stabilizer of the first (n+1)th involutions (in the list of involutions selected out of
      allInvolutions by involutionSelectionIndices)
    - minimumAchievingGroupElement: TODO: explain.
    */
    template <size_t N>
    MinimalStatus isSetOfInvolutionsMinimalHelper(const std::vector<SymmetricGroupElement<N>>& allInvolutions, const std::vector<size_t>& involutionSelectionIndices, size_t selectionSize, std::vector<bool>& isIncludedInPermutation, std::vector<size_t>& partialPermutation, int indexIntoPermutation, StabilizerCache<N>& sCache, const SymmetricGroupElement<N>& minimumAchievingGroupElement) {
        SymmetricGroupElement<N> newMinimumAchievingGroupElement("()");
        if (indexIntoPermutation != -1) {
            SymmetricGroupElement<N> minimumInOrbit;
            if (indexIntoPermutation == 0) {
                std::tie(minimumInOrbit, newMinimumAchievingGroupElement) = getMinimalImageOfInvolutionWithGroupElement(allInvolutions[involutionSelectionIndices[partialPermutation[indexIntoPermutation]]]);
            } else {
                if (sCache.cacheFreshUpTo < indexIntoPermutation - 1) { // we assume that sCache.cacheFreshUpTo == indexIntoPermutation - 2
                    if (indexIntoPermutation == 1) {
                        sCache.pointwiseStabilizers[indexIntoPermutation - 1] = getInvolutionStabilizer(allInvolutions[involutionSelectionIndices[0]]);
                    } else {
                        sCache.pointwiseStabilizers[indexIntoPermutation - 1] = getInvolutionStabilizerGivenGroup(allInvolutions[involutionSelectionIndices[indexIntoPermutation - 1]], sCache.pointwiseStabilizers[indexIntoPermutation - 2]);
                    }
                    ++sCache.cacheFreshUpTo;
                }
                bool flag = true;
                SymmetricGroupElement<N> bestGroupElement;
                for (const SymmetricGroupElement<N>& stabilizerElement : sCache.pointwiseStabilizers[indexIntoPermutation - 1]) {
                    const SymmetricGroupElement<N> rightCosetMember = stabilizerElement * minimumAchievingGroupElement;
                    const SymmetricGroupElement<N> image = conjugate(allInvolutions[involutionSelectionIndices[partialPermutation[indexIntoPermutation]]], rightCosetMember); if (flag || image < minimumInOrbit) {
                        minimumInOrbit = image;
                        bestGroupElement = rightCosetMember;
                    }
                    flag = false;
                }
                newMinimumAchievingGroupElement = bestGroupElement;
            }
            if (minimumInOrbit < allInvolutions[involutionSelectionIndices[indexIntoPermutation]]) {
                return MinimalStatus::NotMinimal;
            } else if (minimumInOrbit > allInvolutions[involutionSelectionIndices[indexIntoPermutation]]) {
                return MinimalStatus::Undetermined;
            }
        }
        for (size_t i = 0; i < selectionSize; ++i) {
            if (!isIncludedInPermutation[i]) {
                isIncludedInPermutation[i] = true;
                partialPermutation[indexIntoPermutation + 1] = i;
                MinimalStatus status = isSetOfInvolutionsMinimalHelper(allInvolutions, involutionSelectionIndices, selectionSize, isIncludedInPermutation, partialPermutation, indexIntoPermutation + 1, sCache, newMinimumAchievingGroupElement);
                if (status == MinimalStatus::NotMinimal) {
                    return MinimalStatus::NotMinimal;
                }
                isIncludedInPermutation[i] = false;
            }
        }
        return MinimalStatus::Undetermined;
    }

    template <size_t N>
    bool isSetOfInvolutionsMinimal(const std::vector<SymmetricGroupElement<N>>& allInvolutions, const std::vector<size_t>& involutionSelectionIndices, size_t selectionSize, StabilizerCache<N>& sCache) {
        std::vector<bool> isIncludedInPermutation(selectionSize, false);
        std::vector<size_t> partialPermutation(selectionSize);
        return isSetOfInvolutionsMinimalHelper(allInvolutions, involutionSelectionIndices, selectionSize, isIncludedInPermutation, partialPermutation, -1, sCache, SymmetricGroupElement<N>("()")) != MinimalStatus::NotMinimal;
    }
}

#endif