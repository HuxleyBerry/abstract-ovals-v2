#ifndef MINIMAL_IMAGE_H
#define MINIMAL_IMAGE_H

#include <vector>
#include <array>
#include <utility>
#include <cstdint>
#include "../SymmetricGroupElement.h"

std::vector<std::vector<size_t>> getAllPermutations(int permSize);

template <size_t N>
std::vector<std::pair<int,int>> getMatchingsFromInvolution(const std::array<int, N>& arrayRep) {
    //TODO: fix bug where we shouldn't be counting a pair of fixed points as a matching
    std::vector<std::pair<int,int>> matchings;
    matchings.reserve(N/2);
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
                matchings.emplace_back(i, image);
                bitset |= (1 << image);
            }
        }
    }
    if (firstFixed != -1 && secondFixed != -1) {
        matchings.emplace_back(firstFixed, secondFixed);
    }
    return matchings;
}

// assumes involution has either zero, one, or two fixed points
// TODO: consider if a BSGS would be more efficient rather than computing list of every element
template <size_t N>
std::vector<SymmetricGroupElement<N>> getInvolutionStabilizer(const SymmetricGroupElement<N>& inv) {
    std::vector<SymmetricGroupElement<N>> stabilizingPermutations;
    std::vector<std::pair<int,int>> matchings = getMatchingsFromInvolution(inv.getInternalArray());
    // we should have matchings.size() == N/2
    int tempCount = 0;
    for (const std::vector<size_t>& innerPerm: getAllPermutations(N/2)) {
        ++tempCount;
        if (tempCount == 2) {
            std::cout << "innerPerm: " << innerPerm[0] << " " << innerPerm[1] << " " << innerPerm[2] << "\n";
        }
        for (int transpositionSelection = 0; transpositionSelection < (1 << (N/2)); ++transpositionSelection) {
            std::array<int, N> perm;
            // TODO: consider optmising this loop out
            for (size_t i = 0; i < N; ++i) {
                perm[i] = i;
            }
            for (size_t j = 0; j < N/2; ++j) {
                if (((transpositionSelection >> j) & 1) == 0) { // jth bit 0
                    //std::cout << matchings[j].first << " " << matchings[innerPerm[j]].first << "\n";
                    //std::cout << "innerPerm: " << innerPerm[0] << " " << innerPerm[1] << " " << innerPerm[2] << "\n";
                    perm[matchings[j].first] = matchings[innerPerm[j]].first;
                    perm[matchings[j].second] = matchings[innerPerm[j]].second;
                } else {
                    perm[matchings[j].first] = matchings[innerPerm[j]].second;
                    perm[matchings[j].second] = matchings[innerPerm[j]].first;
                }
            }
            stabilizingPermutations.push_back(SymmetricGroupElement<N>(std::move(perm)));
        }
    }
    return stabilizingPermutations;
}

// Assumes involution has either zero, one, or two fixed points.
// finds the minimal member of the orbit of the given involution under the conjugation action of
// S_N. The notion of "minimal" uses an ordering on the permutation of S_N which is just tuple
// comparison of the array representation of the permutation.
template <size_t N>
SymmetricGroupElement<N> getMinimalImageOfInvolution(const SymmetricGroupElement<N>& inv, int fixedPointCount) {
    std::array<int, N> perm;
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

#endif