#ifndef ABSTRACT_OVAL_FINDER_H
#define ABSTRACT_OVAL_FINDER_H

#include <vector>
#include "SymmetricGroupElement.h"
#include "AbstractOvalUtils.h"
#include "exact-cover-solvers/AlgorithmDLX_V2.h"
#include "exact-cover-solvers/IncreasingOrderDLX.h"
#include "orderly/MinimalImage.h"

template <size_t N>
std::vector<std::vector<SymmetricGroupElement<N + 1>>> basicAbstractOvalFinder(bool includeIdentity) {
    std::vector<SymmetricGroupElement<N + 1>> candidatePermutations(getAbstractOvalCandidatePermutations<N>());
    std::vector<FourPoints> quadruplesToBeCovered(getPointCombinationsForAbstractOval(N));
    auto covering = getCovering<N>(candidatePermutations, quadruplesToBeCovered);
    std::vector<std::vector<int>> solutionsAsIndices = algorithmX_V2(covering, quadruplesToBeCovered.size());
    return involutionIndicesToAbstractOvals<N>(solutionsAsIndices, candidatePermutations, includeIdentity);
}

template <size_t N>
std::vector<std::vector<SymmetricGroupElement<N + 1>>> basicAbstractOvalFinder() {
    return basicAbstractOvalFinder<N>(true);
}

template <size_t N>
std::vector<std::vector<SymmetricGroupElement<N + 1>>> minimalAbstractOvalFinder() {
    // don't include the identity in the abtract ovals since it messes with the minimality checking
    // we add the identity permutation in later.
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> abstractOvalsIncludingIsomorphicCopies = basicAbstractOvalFinder<N>(false);
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> filteredAbstractOvals;
    for (std::vector<SymmetricGroupElement<N + 1>>& abstractOval : abstractOvalsIncludingIsomorphicCopies) {
        std::sort(abstractOval.begin(), abstractOval.end());
        if (isSetOfInvolutionsMinimal(abstractOval)) {
            if (N%2 == 0) {
                abstractOval.emplace_back("()");
            }
            filteredAbstractOvals.push_back(abstractOval);
        }
    }
    return filteredAbstractOvals;
}

void printAllAbstractOvalsWithIsomorphsRemoved(int order);

template <size_t N> void printAllAbstractOvals() {
    printAllAbstractOvals<N>([](){ return basicAbstractOvalFinder<N>();});
}

template <size_t N> void printAllAbstractOvalsWithIsomorphsRemoved() {
    printAllAbstractOvals<N>([](){ return minimalAbstractOvalFinder<N>();});
}

#endif