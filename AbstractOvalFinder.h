#ifndef ABSTRACT_OVAL_FINDER_H
#define ABSTRACT_OVAL_FINDER_H

#include <vector>
#include "SymmetricGroupElement.h"
#include "AbstractOvalUtils.h"
#include "exact-cover-solvers/dancing-links.h"

template <int N>
std::vector<std::vector<SymmetricGroupElement<N + 1>>> basicAbstractOvalFinder() {
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
        if (N%2 == 0) {
            asPerm.emplace_back("()");
        }
        return asPerm;
    });
    return solutionsAsPermutations;
}

void printAllAbstractOvals(int order);

template <int N> void printAllAbstractOvals() {
    std::cout << "==================== Abstract Ovals of order " << std::to_string(N) << " ====================\n";
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> abstractOvals = basicAbstractOvalFinder<N>();
    for (const std::vector<SymmetricGroupElement<N + 1>>& oval : abstractOvals) {
        std::cout << listOfPermutationsToString<N + 1>(oval) << "\n";
    }
}

#endif