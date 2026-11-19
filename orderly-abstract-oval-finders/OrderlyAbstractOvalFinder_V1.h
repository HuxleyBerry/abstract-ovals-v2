#ifndef ORDERLY_ABSTRACT_OVAL_FINDER_V1_H
#define ORDERLY_ABSTRACT_OVAL_FINDER_V1_H

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <optional>
#include "MinimalImageWithCache.h"
#include "../orderly/MinimalImage.h"
#include "../AbstractOvalUtils.h"
#include "../SymmetricGroupElement.h"
#include "../exact-cover-solvers/IncreasingOrderShared.h"


template <size_t order>
void orderlyExactCoverSolverHelper(const std::vector<SymmetricGroupElement<order + 1>>& candidateInvolutions, std::vector<Node>& nodes, size_t currentOptionNodeIndex, int level, const std::map<size_t, size_t>& optionNodeIndexToOptionIndex, std::vector<size_t>& partialSolution, std::vector<std::vector<int>>& solutions, MinimalImageWithCache::StabilizerCache<order + 1>& sCache, const std::optional<unsigned int>& orderlyUpTo) {
    //printAllNodes(nodes);
    if (nodes[0].R == 0) { // all items covered
        //std::cout << "SOLUTION FOUND\n";
        solutions.emplace_back(partialSolution.cbegin(), partialSolution.cbegin() + level);
        return;
    }

    std::optional<size_t> indexOfNodeInFinalPlausibleOption = getFinalPlausibleIndex(nodes);
    if (!indexOfNodeInFinalPlausibleOption) {
        return;
    }

    if (level != 0) {
        // level is the length of the current partial solution, so one more than the index most recently set in the partial solution
        // Therefore, we just changed the partial solution at index (level - 1)
        // The stabilizers will still be valid up until index (level - 2)
        sCache.cacheFreshUpTo = std::min(sCache.cacheFreshUpTo, level - 2);
        if ((!orderlyUpTo || level <= orderlyUpTo.value()) && !MinimalImageWithCache::isSetOfInvolutionsMinimal<order + 1>(candidateInvolutions, partialSolution, level, sCache)) {
            //std::cout << "level " << level << ": pruning due to being non-minimal\n";
            return;
        }
    }
    
    while (currentOptionNodeIndex != 0) {
        if (currentOptionNodeIndex > indexOfNodeInFinalPlausibleOption) {
            break;
        }
        //std::cout << "Selecting option " << optionNodeIndexToOptionIndex.at(currentOptionNodeIndex) << " at level " << level << "\n";
        size_t currIndexWithinOption = currentOptionNodeIndex + 1;
        while (nodes[currIndexWithinOption].type == 2) {
            //std::cout << "calling cover on index node " << nodes[currIndexWithinOption].L << "\n";
            cover(nodes, nodes[currIndexWithinOption].L, currentOptionNodeIndex);
            ++currIndexWithinOption;
        }

        size_t next = nodes[currentOptionNodeIndex].D;

        partialSolution[level] = optionNodeIndexToOptionIndex.at(currentOptionNodeIndex);
        orderlyExactCoverSolverHelper<order>(candidateInvolutions, nodes, next, level + 1, optionNodeIndexToOptionIndex, partialSolution, solutions, sCache, orderlyUpTo);

        --currIndexWithinOption;
        while (nodes[currIndexWithinOption].type == 2) {
            uncover(nodes, nodes[currIndexWithinOption].L, currentOptionNodeIndex);
            --currIndexWithinOption;
        }

        currentOptionNodeIndex = nodes[currentOptionNodeIndex].D;
    }
}

template <size_t order>
std::vector<std::vector<int>> orderlyExactCoverSolver(const std::vector<SymmetricGroupElement<order + 1>>& candidateInvolutions, const std::vector<std::vector<int>>& coveringData, int itemCount, const std::optional<unsigned int>& orderlyUpTo) {
    std::vector<std::vector<int>> solutions;
    auto [nodes, optionNodeIndexToOptionIndex] = makeLinkStructure<Node>(coveringData, itemCount);
    std::vector<size_t> partialSolution(itemCount); // assumes every option covers at least one item.
    MinimalImageWithCache::StabilizerCache<order + 1> sCache(orderlyUpTo.value_or((order + 1) * (order + 1)));
    orderlyExactCoverSolverHelper<order>(candidateInvolutions, nodes, nodes[0].D, 0, optionNodeIndexToOptionIndex, partialSolution, solutions, sCache, orderlyUpTo);
    return solutions;
}

template <size_t order>
std::vector<std::vector<SymmetricGroupElement<order + 1>>> orderlyAbstractOvalFinder(const std::optional<unsigned int>& orderlyUpTo) {
    std::vector<SymmetricGroupElement<order + 1>> candidateInvolutions(getAbstractOvalCandidatePermutations<order>());
    // need to sort to be compatible with the minimal image checking
    std::sort(candidateInvolutions.begin(), candidateInvolutions.end());
    std::vector<FourPoints> quadruplesToBeCovered(getPointCombinationsForAbstractOval(order));
    auto coveringData = getCovering<order>(candidateInvolutions, quadruplesToBeCovered);
    std::vector<std::vector<int>> solutionsAsIndices = orderlyExactCoverSolver<order>(candidateInvolutions, coveringData, quadruplesToBeCovered.size(), orderlyUpTo);
    // only append the identity if orderlyUpTo has no value because otherwise we need to filter for minimality
    // and the minimality check assumes no identity.
    return involutionIndicesToAbstractOvals<order>(solutionsAsIndices, candidateInvolutions, !orderlyUpTo);
}

template <size_t order>
std::vector<std::vector<SymmetricGroupElement<order + 1>>> orderlyAbstractOvalFinderNoIsomorphs(const std::optional<unsigned int>& orderlyUpTo = std::nullopt) {
    auto unfiltered = orderlyAbstractOvalFinder<order>(orderlyUpTo);
    if (!orderlyUpTo) {
        return unfiltered;
    }
    std::cout << "Abstract Ovals found, now filtering out isomorphs\n";
    std::vector<std::vector<SymmetricGroupElement<order + 1>>> filtered;
    for (auto& invList: unfiltered) {
        if (isSetOfInvolutionsMinimal<order + 1>(invList)) {
            if (orderlyUpTo && order%2 == 1) {
                invList.emplace_back("()");
            }
            filtered.push_back(std::move(invList));
        }
    }
    return filtered;
}

template <size_t N>
void printAllAbstractOvals(const std::optional<unsigned int>& orderlyUpTo = std::nullopt) {
    printAllAbstractOvals<N>([&](){ return orderlyAbstractOvalFinderNoIsomorphs<N>(orderlyUpTo); });
}

void printAllAbstractOvalsFoundFromOrderlyAlgorithm(int order);

#endif