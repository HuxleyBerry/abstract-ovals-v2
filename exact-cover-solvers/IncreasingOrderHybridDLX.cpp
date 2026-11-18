#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <optional>
#include "IncreasingOrderShared.h"
#include "IncreasingOrderHybridDLX.h"

namespace {

    // TODO: fix this. Will need to add a field to Node to store top/use count
    //
    /*
    void algorithmXHelper(std::vector<Node>& nodes, int level, std::vector<int>& partialSolution, std::vector<std::vector<int>>& solutions) {
        if (nodes[0].R == 0) { // all items covered
            // note a copy of partialSolution is made.
            solutions.emplace_back(partialSolution.cbegin(), partialSolution.cbegin() + level);
            return;
        }
        bool flag = true;
        size_t minOptionsCovering;
        size_t currentNodeIndex = nodes[0].R;
        size_t indexOfChosenItem;
        while (currentNodeIndex != 0) {
            if (flag || nodes[currentNodeIndex].top < minOptionsCovering) {
                flag = false;
                minOptionsCovering = nodes[currentNodeIndex].top;
                indexOfChosenItem = currentNodeIndex;
            }
            currentNodeIndex = nodes[currentNodeIndex].R;
        }
        cover(nodes, indexOfChosenItem);

        size_t currentOptionNodeIndex = nodes[indexOfChosenItem].D;
        while (currentOptionNodeIndex != indexOfChosenItem) {
            //std::cout << "covering item " << indexOfChosenItem << " with option " << nodes[currentOptionNodeIndex].L << "\n";
            // now cover all the items covered by the current option
            size_t curr = currentOptionNodeIndex + 1;
            while (curr != currentOptionNodeIndex) {
                size_t top = nodes[curr].top;
                if (top == 0) { // spacer
                    curr = nodes[curr].U;
                } else {
                    cover(nodes, top);
                    ++curr;
                }
            }
            partialSolution[level] = nodes[currentOptionNodeIndex].L; // recall that on a non header node, the L property is used to point to the associated option.
            algorithmXHelper(nodes, level + 1, partialSolution, solutions);
            //std::cout << "uncovering\n";
            // undo the covering
            curr = currentOptionNodeIndex - 1;
            while (curr != currentOptionNodeIndex) {
                size_t top = nodes[curr].top;
                if (top == 0) { // spacer
                    curr = nodes[curr].D;
                } else {
                    uncover(nodes, top);
                    --curr;
                }
            }
            currentOptionNodeIndex = nodes[currentOptionNodeIndex].D;
        }

        uncover(nodes, indexOfChosenItem);
    }
    */

    void increasingOrderHybridSolver_Helper(std::vector<Node>& nodes, size_t currentOptionNodeIndex, int level, const std::map<size_t, size_t>& optionNodeIndexToOptionIndex, std::vector<int>& partialSolution, std::vector<std::vector<int>>& solutions, size_t sizeToTransitionAt) {
        //printAllNodes(nodes);
        if (nodes[0].R == 0) { // all items covered
            //std::cout << "SOLUTION FOUND\n";
            solutions.emplace_back(partialSolution.cbegin(), partialSolution.cbegin() + level);
            return;
        }

        if (level < sizeToTransitionAt) {
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
                increasingOrderHybridSolver_Helper(nodes, next, level + 1, optionNodeIndexToOptionIndex, partialSolution, solutions, sizeToTransitionAt);

                nodes[nodes[currentOptionNodeIndex].U].D = currentOptionNodeIndex;
                nodes[nodes[currentOptionNodeIndex].D].U = currentOptionNodeIndex;

                --currIndexWithinOption;
                while (nodes[currIndexWithinOption].type == 2) {
                    uncover(nodes, nodes[currIndexWithinOption].L, currentOptionNodeIndex);
                    --currIndexWithinOption;
                }

                currentOptionNodeIndex = nodes[currentOptionNodeIndex].D;
            }
        } else {
            //algorithmXHelper(nodes, level, partialSolution, solutions);
        }
    }
}

std::vector<std::vector<int>> increasingOrderHybridSolver(const std::vector<std::vector<int>>& data, int itemCount, size_t sizeToTransitionAt) {
    std::vector<std::vector<int>> solutions;
    auto [nodes, optionNodeIndexToOptionIndex] = makeLinkStructure(data, itemCount);
    std::vector<int> partialSolution(itemCount); // assumes every option covers at least one item.
    increasingOrderHybridSolver_Helper(nodes, nodes[0].D, 0, optionNodeIndexToOptionIndex, partialSolution, solutions, sizeToTransitionAt);
    return solutions;
}