#ifndef ABSTRACT_OVAL_FINDER_H
#define ABSTRACT_OVAL_FINDER_H

#include <vector>
#include "SymmetricGroupElement.h"
#include "AbstractOvalUtils.h"
#include "exact-cover-solvers/dancing-links.h"
#include "orderly/MinimalImage.h"

template <size_t N>
std::vector<std::vector<SymmetricGroupElement<N + 1>>> basicAbstractOvalFinderExcludingIdentity() {
    std::vector<SymmetricGroupElement<N + 1>> candidatePermutations(getAbstractOvalCandidatePermutations<N>());
    std::vector<FourPoints> quadruplesToBeCovered(getPointCombinationsForAbstractOval(N));
    auto covering = getCovering<N>(candidatePermutations, quadruplesToBeCovered);
    std::vector<std::vector<int>> solutionsAsIndices = algorithmX(covering, quadruplesToBeCovered.size());
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> solutionsAsPermutations;
    std::transform(solutionsAsIndices.cbegin(), solutionsAsIndices.cend(), std::back_inserter(solutionsAsPermutations), [&candidatePermutations](const std::vector<int> sol){
        std::vector<SymmetricGroupElement<N + 1>> asPerm;
        std::transform(sol.cbegin(), sol.cend(), std::back_inserter(asPerm), [&candidatePermutations](int i){
            return candidatePermutations[i];
        });
        return asPerm;
    });
    return solutionsAsPermutations;
}

template <size_t N>
std::vector<std::vector<SymmetricGroupElement<N + 1>>> basicAbstractOvalFinder() {
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> abstractOvals = basicAbstractOvalFinderExcludingIdentity<N>();
    if (N%2 == 0) {
        for (std::vector<SymmetricGroupElement<N + 1>>& almostCompleteOval : abstractOvals) {
            almostCompleteOval.emplace_back("()");
        }
    }
    return abstractOvals;
}

template <size_t N>
std::vector<std::vector<SymmetricGroupElement<N + 1>>> minimalAbstractOvalFinder() {
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> abstractOvalsIncludingIsomorphicCopies = basicAbstractOvalFinderExcludingIdentity<N>();
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
    std::cout << "==================== Abstract Ovals of order " << std::to_string(N) << " ====================\n";
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> abstractOvals = basicAbstractOvalFinder<N>();
    for (const std::vector<SymmetricGroupElement<N + 1>>& oval : abstractOvals) {
        std::cout << listOfPermutationsToString<N + 1>(oval) << "\n";
    }
}

template <size_t N> void printAllAbstractOvalsWithIsomorphsRemoved() {
    std::cout << "==================== Abstract Ovals of order " << std::to_string(N) << " ====================\n";
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> abstractOvals = minimalAbstractOvalFinder<N>();
    for (const std::vector<SymmetricGroupElement<N + 1>>& oval : abstractOvals) {
        std::cout << listOfPermutationsToString<N + 1>(oval) << "\n";
    }
}

#endif