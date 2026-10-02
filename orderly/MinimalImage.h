#ifndef MINIMAL_IMAGE_H
#define MINIMAL_IMAGE_H

#include <vector>
#include <array>
#include <utility>
#include <cstdint>
#include <optional>
#include <algorithm>
#include <tuple>
#include "../SymmetricGroupElement.h"

std::vector<std::vector<size_t>> getAllPermutations(int permSize);

struct InvolutionStructure {
    std::vector<std::pair<int,int>> pairs;
    std::pair<int, int> fixedPoints; //guaranteed to be at most two fixed points.
};

template <size_t N>
InvolutionStructure getInvolutionStructure(const std::array<size_t, N>& arrayRep) {
    std::vector<std::pair<int,int>> pairs;
    std::uint16_t bitset = 0;
    int firstFixed = -1;
    int secondFixed = -1;
    for (int i = 0; i < N; ++i) {
        if (((bitset >> i) & 1) == 0) {
            bitset |= (1 << i);
            int image = arrayRep[i];
            if (i == image) { // fixed point
                if (firstFixed == -1) {
                    firstFixed = i;
                } else {
                    secondFixed = i;
                }
            } else {
                pairs.emplace_back(i, image);
                bitset |= (1 << image);
            }
        }
    }
    return { pairs, std::pair(firstFixed, secondFixed) };
}

// assumes involution has either zero, one, or two fixed points
// TODO: consider if a BSGS would be more efficient rather than computing list of every element
template <size_t N>
std::vector<SymmetricGroupElement<N>> getInvolutionStabilizer(const SymmetricGroupElement<N>& inv) {
    std::vector<SymmetricGroupElement<N>> stabilizingPermutations;
    const InvolutionStructure structure = getInvolutionStructure(inv.getInternalArray());
    size_t pairCount = structure.pairs.size();
    for (const std::vector<size_t>& innerPerm: getAllPermutations(pairCount)) {
        for (int transpositionSelection = 0; transpositionSelection < (1 << (pairCount)); ++transpositionSelection) {
            std::array<size_t, N> perm;
            for (size_t j = 0; j < pairCount; ++j) {
                if (((transpositionSelection >> j) & 1) == 0) { // jth bit 0
                    perm[structure.pairs[j].first] = structure.pairs[innerPerm[j]].first;
                    perm[structure.pairs[j].second] = structure.pairs[innerPerm[j]].second;
                } else {
                    perm[structure.pairs[j].first] = structure.pairs[innerPerm[j]].second;
                    perm[structure.pairs[j].second] = structure.pairs[innerPerm[j]].first;
                }
            }
            if (structure.fixedPoints.first != -1) {
                if (structure.fixedPoints.second != -1) {
                    std::array<size_t, N> perm2(perm);
                    perm[structure.fixedPoints.first] = structure.fixedPoints.second;
                    perm[structure.fixedPoints.second] = structure.fixedPoints.first;
                    perm2[structure.fixedPoints.first] = structure.fixedPoints.first;
                    perm2[structure.fixedPoints.second] = structure.fixedPoints.second;
                    stabilizingPermutations.push_back(SymmetricGroupElement<N>(std::move(perm2)));
                } else {
                    perm[structure.fixedPoints.first] = structure.fixedPoints.first;
                }
            }
            stabilizingPermutations.push_back(SymmetricGroupElement<N>(std::move(perm)));
        }
    }
    return stabilizingPermutations;
}

template <size_t N>
std::vector<SymmetricGroupElement<N>> getInvolutionStabilizerGivenGroup(const SymmetricGroupElement<N>& inv, const std::vector<SymmetricGroupElement<N>>& listOfAllGroupElements) {
    std::vector<SymmetricGroupElement<N>> stab;
    for (const SymmetricGroupElement<N>& el : listOfAllGroupElements) {
        if (el * inv == inv * el) {
            stab.push_back(el);
        }
    }
    return stab;
}

// Assumes involution has either zero, one, or two fixed points.
// finds the minimal member of the orbit of the given involution under the conjugation action of
// S_N. The notion of "minimal" uses an ordering on the permutation of S_N which is just tuple
// comparison of the array representation of the permutation.
template <size_t N>
SymmetricGroupElement<N> getMinimalImageOfInvolution(const SymmetricGroupElement<N>& inv, int fixedPointCount) {
    std::array<size_t, N> perm;
    int startOfNotFixed = 0;
    for (int i = 0; i < fixedPointCount; ++i) {
        perm[i] = i;
        ++startOfNotFixed;
    }
    for (int i = startOfNotFixed; i < N; i += 2) {
        perm[i] = i + 1;
        perm[i + 1] = i;
    }
    return SymmetricGroupElement<N>(std::move(perm));
}

// Assumes involution has either zero, one, or two fixed points.
// returns a pair (g,h) such that inv^h = g.
template <size_t N>
std::pair<SymmetricGroupElement<N>,SymmetricGroupElement<N>> getMinimalImageOfInvolutionWithGroupElement(const SymmetricGroupElement<N>& inv) {
    std::array<size_t, N> arrayForMinimal;
    std::array<size_t, N> arrayForPermutationAchievingMinimal;
    InvolutionStructure structure = getInvolutionStructure(inv.getInternalArray());
    int startOfNotFixed = 0;
    if (structure.fixedPoints.first != -1) {
        arrayForMinimal[0] = 0;
        arrayForPermutationAchievingMinimal[structure.fixedPoints.first] = 0;
        ++startOfNotFixed;
    }
    if (structure.fixedPoints.second != -1) {
        arrayForMinimal[1] = 1;
        arrayForPermutationAchievingMinimal[structure.fixedPoints.second] = 1;
        ++startOfNotFixed;
    }
    int pairIndex = 0;
    for (int i = startOfNotFixed; i < N; i += 2) {
        arrayForMinimal[i] = i + 1;
        arrayForMinimal[i + 1] = i;
        arrayForPermutationAchievingMinimal[structure.pairs[pairIndex].first] = i;
        arrayForPermutationAchievingMinimal[structure.pairs[pairIndex].second] = i + 1;
        ++pairIndex;
    }
    return std::pair(SymmetricGroupElement<N>(std::move(arrayForMinimal)), SymmetricGroupElement<N>(std::move(arrayForPermutationAchievingMinimal)));
}

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
    //std::cout << indexIntoPermutation << "\n";
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
                    pointwiseStabilisers.push_back(getInvolutionStabilizerGivenGroup(involutions[indexIntoPermutation], pointwiseStabilisers[indexIntoPermutation - 2]));
                }
            }
            bool flag = true;
            SymmetricGroupElement<N> bestGroupElement;
            //std::cout << "the permutation is: " << partialPermutation[0] << " " << partialPermutation[1] << "\n";
            //std::cout << "the stabilizer is: " << listOfPermutationsToString(pointwiseStabilisers[indexIntoPermutation - 1]) << "\n";
            for (const SymmetricGroupElement<N>& stabilizerElement : pointwiseStabilisers[indexIntoPermutation - 1]) {
                const SymmetricGroupElement<N> leftCosetMember = minimumAchievingGroupElement * stabilizerElement;
                const SymmetricGroupElement<N> image = congjugate(involutions[partialPermutation[indexIntoPermutation]], leftCosetMember);
                //std::cout << "Conjugating " << involutions[partialPermutation[indexIntoPermutation]].toString() << " by " << leftCosetMember.toString() << " resulted in " << image.toString() << "\n";
                if (flag || image < minimumInOrbit) {
                    minimumInOrbit = image;
                    bestGroupElement = leftCosetMember;
                }
                flag = false;
            }
            newMinimumAchievingGroupElement = bestGroupElement;
        }
        if (minimumInOrbit < involutions[indexIntoPermutation]) {
            //std::cout << "found not minimal: " << minimumInOrbit.toString() << " < " << involutions[indexIntoPermutation].toString() << "\n";
            return MinimalStatus::NotMinimal;
        } else if (minimumInOrbit > involutions[indexIntoPermutation]) {
            //std::cout << "found undetermined\n";
            return MinimalStatus::Undetermined;
        }
    }
    //newMinimumAchievingGroupElement.printInternalArray();
    for (int i = 0; i < involutions.size(); ++i) {
        if (!isIncludedInPermutation[i]) {
            isIncludedInPermutation[i] = true;
            //std::cout << "adding " << i << " to index " << indexIntoPermutation + 1 << " of partial permutation\n";
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