#ifndef ORDERLY_ABSTRACT_OVAL_FINDER_V1_H
#define ORDERLY_ABSTRACT_OVAL_FINDER_V1_H

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <map>
#include <utility>
#include <optional>
#include "../orderly/MinimalImage.h"
#include "../AbstractOvalUtils.h"
#include "../SymmetricGroupElement.h"

namespace OAOF {
    struct Node {
        std::uint32_t L = 0;
        std::uint32_t R = 0;
        std::uint32_t U = 0;
        std::uint32_t D = 0;
        std::uint8_t type;

        // In a header node, the properties are named "sensibly", referring to the Left, Right, Up and Down links.
        // In an option node, we overload as follows:
        // - L is the index of the previous option node. This is slightly different from U because U will change over time 
        //   as items are removed from the vertical list of option nodes.
        // - R is the index of the next option node
        // In a normal node, we overload as follows:
        // - L is the index of the header node of the vertical list
        // - R is unused

        Node(std::uint8_t t): type(t) {}
    };

    void addToEndOfHorizontalDoublyLinkedList(std::vector<Node>& nodeStorage, size_t startIndex, size_t toAddIndex);

    void addToEndOfVerticalDoublyLinkedList(std::vector<Node>& nodeStorage, size_t startIndex, size_t toAddIndex);

    std::pair<std::vector<Node>, std::map<size_t, size_t>> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount);

    void hideOption(std::vector<Node>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);

    void unhideOption(std::vector<Node>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);

    void cover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);

    void uncover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide) ;

    std::string nodeToString(const Node& node);

    void printAllNodes(const std::vector<Node>& nodes);

    std::optional<size_t> getFinalPlausibleIndex(std::vector<Node>& nodes);

    template <size_t order>
    void orderlyExactCoverSolverHelper(const std::vector<SymmetricGroupElement<order + 1>>& candidateInvolutions, std::vector<Node>& nodes, size_t currentOptionNodeIndex, int level, const std::map<size_t, size_t>& optionNodeIndexToOptionIndex, std::vector<size_t>& partialSolution, std::vector<std::vector<int>>& solutions) {
        //printAllNodes(nodes);
        if (nodes[0].R == 0) { // all items covered
            //std::cout << "SOLUTION FOUND\n";
            solutions.emplace_back(partialSolution.cbegin(), partialSolution.cbegin() + level);
            return;
        }

        if (level != 0 && !isSetOfInvolutionsMinimal(candidateInvolutions, partialSolution, level)) {
            std::cout << "not minimal, pruning!\n";
            return;
        }

        std::optional<size_t> indexOfNodeInFinalPlausibleOption = getFinalPlausibleIndex(nodes);
        if (!indexOfNodeInFinalPlausibleOption) {
            return;
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
            nodes[nodes[currentOptionNodeIndex].U].D = nodes[currentOptionNodeIndex].D;
            nodes[nodes[currentOptionNodeIndex].D].U = nodes[currentOptionNodeIndex].U;

            partialSolution[level] = optionNodeIndexToOptionIndex.at(currentOptionNodeIndex);
            orderlyExactCoverSolverHelper<order>(candidateInvolutions, nodes, next, level + 1, optionNodeIndexToOptionIndex, partialSolution, solutions);

            nodes[nodes[currentOptionNodeIndex].U].D = currentOptionNodeIndex;
            nodes[nodes[currentOptionNodeIndex].D].U = currentOptionNodeIndex;

            --currIndexWithinOption;
            while (nodes[currIndexWithinOption].type == 2) {
                uncover(nodes, nodes[currIndexWithinOption].L, currentOptionNodeIndex);
                --currIndexWithinOption;
            }

            currentOptionNodeIndex = nodes[currentOptionNodeIndex].D;
        }
    }

    template <size_t order>
    std::vector<std::vector<int>> orderlyExactCoverSolver(const std::vector<SymmetricGroupElement<order + 1>>& candidateInvolutions, const std::vector<std::vector<int>>& coveringData, int itemCount) {
        std::vector<std::vector<int>> solutions;
        auto [nodes, optionNodeIndexToOptionIndex] = makeLinkStructure(coveringData, itemCount);
        std::vector<size_t> partialSolution(itemCount); // assumes every option covers at least one item.
        orderlyExactCoverSolverHelper<order>(candidateInvolutions, nodes, nodes[0].D, 0, optionNodeIndexToOptionIndex, partialSolution, solutions);
        return solutions;
    }

    template <size_t order>
    std::vector<std::vector<SymmetricGroupElement<order + 1>>> orderlyAbstractOvalFinder() {
        std::vector<SymmetricGroupElement<order + 1>> candidateInvolutions(getAbstractOvalCandidatePermutations<order>());
        // need to sort to be compatible with the minimal image checking
        std::sort(candidateInvolutions.begin(), candidateInvolutions.end());
        std::vector<FourPoints> quadruplesToBeCovered(getPointCombinationsForAbstractOval(order));
        auto coveringData = getCovering<order>(candidateInvolutions, quadruplesToBeCovered);
        std::vector<std::vector<int>> solutionsAsIndices = orderlyExactCoverSolver<order>(candidateInvolutions, coveringData, quadruplesToBeCovered.size());
        return involutionIndicesToAbstractOvals<order>(solutionsAsIndices, candidateInvolutions, true);
    }

    template <size_t N>
    void printAllAbstractOvals() {
        ::printAllAbstractOvals<N>([](){ return orderlyAbstractOvalFinder<N>(); });
    }

    void printAllAbstractOvalsFoundFromOrderlyAlgorithm(int order);
}

#endif