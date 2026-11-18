#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <optional>
#include "IncreasingOrderShared.h"
#include "IncreasingOrderDLX.h"

namespace {

    void algorithmX_V2_Helper(std::vector<Node>& nodes, size_t currentOptionNodeIndex, int level, const std::map<size_t, size_t>& optionNodeIndexToOptionIndex, std::vector<int>& partialSolution, std::vector<std::vector<int>>& solutions) {
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
            algorithmX_V2_Helper(nodes, next, level + 1, optionNodeIndexToOptionIndex, partialSolution, solutions);

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
}

std::vector<std::vector<int>> increasingOrderSolver(const std::vector<std::vector<int>>& data, int itemCount) {
    std::vector<std::vector<int>> solutions;
    auto [nodes, optionNodeIndexToOptionIndex] = makeLinkStructure(data, itemCount);
    std::vector<int> partialSolution(itemCount); // assumes every option covers at least one item.
    algorithmX_V2_Helper(nodes, nodes[0].D, 0, optionNodeIndexToOptionIndex, partialSolution, solutions);
    return solutions;
}