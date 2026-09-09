#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "SymmetricGroupElement.h"
#include "SymmetricGroupUtils.h"
#include "AbstractOvalUtils.h"
#include "exact-cover-solvers/dancing-links.h"

std::string vectorToString(std::vector<int> v) {
    std::ostringstream ss;
    ss << "[";
    for (int num: v) {
        ss << num << ",";
    }
    ss << "]";
    return ss.str();
}

void printVector(std::vector<int> v) {
    std::cout << vectorToString(v) << "\n";
}

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
        return asPerm;
    });
    return solutionsAsPermutations;
}

template <int N> void printAllAbstractOvals() {
    std::cout << "==================== Abstract Ovals of order " << std::to_string(N) << " ====================\n";
    std::vector<std::vector<SymmetricGroupElement<N + 1>>> abstractOvals = basicAbstractOvalFinder<N>();
    for (const std::vector<SymmetricGroupElement<N + 1>>& oval : abstractOvals) {
        std::cout << listOfPermutationsToString<N + 1>(oval) << "\n";
    }
}

int main() {
    printAllAbstractOvals<2>();
    printAllAbstractOvals<3>();
    printAllAbstractOvals<4>();
    printAllAbstractOvals<5>();
    printAllAbstractOvals<6>();
    printAllAbstractOvals<7>();
}