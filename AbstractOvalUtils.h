#ifndef ABSTRACT_OVAL_UTILS_H
#define ABSTRACT_OVAL_UTILS_H

#include "SymmetricGroupElement.h"
#include "SymmetricGroupUtils.h"
#include <vector>
#include <array>
#include <utility>

struct FourPoints {
    unsigned int first;
    unsigned int second;
    unsigned int third;
    unsigned int fourth;

    FourPoints(unsigned int i, unsigned int j, unsigned int k,unsigned int n): first(i), second(j), third(k), fourth(n) {}
};

std::vector<FourPoints> getPointCombinationsForAbstractOval(int order);

int quadrupleToInt(const FourPoints& q, int order);

template <int order>
std::vector<SymmetricGroupElement<order + 1>> getAbstractOvalCandidatePermutations() {
    if (order%2 == 0) {
        // we skip the identity permutation;
        std::array<int, order + 1> permArray;
        permArray[order] = order;
        for (int i = 0; i < order/2; ++i) {
            permArray[2 * i] = 2 * i + 1;
            permArray[2 * i + 1] = 2 * i;
        }
        SymmetricGroupElement<order + 1> representative(std::move(permArray));
        return getConjugacyClass(representative);
    } else {
        std::array<int, order + 1> permArray;
        for (int i = 0; i < (order+1)/2; ++i) {
            permArray[2 * i] = 2 * i + 1;
            permArray[2 * i + 1] = 2 * i;
        }
        SymmetricGroupElement<order + 1> zeroFixedRepresentative(permArray);
        permArray[order] = order;
        permArray[order - 1] = order - 1;
        SymmetricGroupElement<order + 1> twoFixedRepresentative(std::move(permArray));
        std::vector<SymmetricGroupElement<order + 1>> zeroFixedConjugacyClass = getConjugacyClass(zeroFixedRepresentative);
        std::vector<SymmetricGroupElement<order + 1>> twoFixedConjugacyClass = getConjugacyClass(twoFixedRepresentative);
        std::cout << zeroFixedConjugacyClass.size() << " " << twoFixedConjugacyClass.size() << "\n";
        zeroFixedConjugacyClass.reserve(zeroFixedConjugacyClass.size() + twoFixedConjugacyClass.size());
        zeroFixedConjugacyClass.insert(zeroFixedConjugacyClass.end(), std::make_move_iterator(twoFixedConjugacyClass.begin()), std::make_move_iterator(twoFixedConjugacyClass.end()));
        return zeroFixedConjugacyClass;
    }
}

template <int order>
std::vector<std::vector<int>> getCovering() {
    std::vector<SymmetricGroupElement<order + 1>> candidatePermutations = getAbstractOvalCandidatePermutations<order>();
    std::vector<int> quadrupleToIndex;
    quadrupleToIndex.reserve((order+1)*(order+1)*(order+1)*(order+1)); //larger than necessary, but allows a simple from quadruples to int;
    std::vector<FourPoints> quadruples(getPointCombinationsForAbstractOval(order));
    for (int i = 0; i < quadruples.size(); ++i) {
        quadrupleToIndex[quadrupleToInt(quadruples[i], order)] = i;
    }

    std::vector<std::vector<int>> ans(candidatePermutations.size());
    for (int i = 0; i < candidatePermutations.size(); ++i) {
        for (int a = 0; a < order + 1; ++a) {
            int aImage = candidatePermutations[i].actOn(a);
            if (aImage >= a) {
                for (int b = a + 1; b < order + 1; ++b) {
                    int bImage = candidatePermutations[i].actOn(b);
                    if (bImage >= b) {
                        ans[i].push_back(quadrupleToIndex[quadrupleToInt(FourPoints(a, aImage, b, bImage), order)]);
                    }
                }
            }
        }
    }
    return ans;
}

// move this function elsewhere?
void printCovering(const std::vector<std::vector<int>>& covering);

#endif