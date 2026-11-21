#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <optional>
#include "IncreasingOrderShared.h"
#include "IncreasingOrderHybridDLX.h"

namespace {

    void hideOption(std::vector<ENode>& nodes, size_t indexForNormalNode, bool removeEncounteredOptionNodes) {
        size_t currentIndex = indexForNormalNode + 1;
        while (currentIndex != indexForNormalNode) {
            if (nodes[currentIndex].type == 1) { // option node
                // Loop back around to the previous option node. i.e. the option node associated with the input normal node.
                currentIndex = nodes[currentIndex].L;
                if (removeEncounteredOptionNodes) {
                    // remove option node from vertical list of option nodes
                    nodes[nodes[currentIndex].U].D = nodes[currentIndex].D;
                    nodes[nodes[currentIndex].D].U = nodes[currentIndex].U;
                }
                ++currentIndex;
            } else {
                nodes[nodes[currentIndex].U].D = nodes[currentIndex].D;
                nodes[nodes[currentIndex].D].U = nodes[currentIndex].U;
                --nodes[nodes[currentIndex].L].count; // decrement counter of remaining options covering item
                ++currentIndex; // proceed to the next node belonging to the current option.
            }
        }
    }

    void unhideOption(std::vector<ENode>& nodes, size_t indexForNormalNode, bool restoreEncounteredOptionNodes) {
        size_t currentIndex = indexForNormalNode - 1;
        while (currentIndex != indexForNormalNode) {
            if (nodes[currentIndex].type == 1) { // option node
                if (restoreEncounteredOptionNodes) {
                    nodes[nodes[currentIndex].U].D = currentIndex;
                    nodes[nodes[currentIndex].D].U = currentIndex;
                }
                currentIndex = nodes[currentIndex].R - 1; // loop back around to the last node belonging to the current option.
            } else {
                nodes[nodes[currentIndex].U].D = currentIndex;
                nodes[nodes[currentIndex].D].U = currentIndex;
                ++nodes[nodes[currentIndex].L].count; // increment counter of remaining options covering item
                --currentIndex; // proceed to the previous node belonging to the current option.
            }
        }
    }

    void hideOptionUsingOptionNode(std::vector<ENode>& nodes, size_t indexForOptionNode) {
        nodes[nodes[indexForOptionNode].U].D = nodes[indexForOptionNode].D;
        nodes[nodes[indexForOptionNode].D].U = nodes[indexForOptionNode].U;
        size_t currentIndex = indexForOptionNode + 1;
        while (nodes[currentIndex].type == 2) {
            nodes[nodes[currentIndex].U].D = nodes[currentIndex].D;
            nodes[nodes[currentIndex].D].U = nodes[currentIndex].U;
            --nodes[nodes[currentIndex].L].count; // decrement counter of remaining options covering item
            ++currentIndex; // proceed to the next node belonging to the current option.
        }
    }

    void unhideOptionUsingOptionNode(std::vector<ENode>& nodes, size_t indexForOptionNode) {
        size_t currentIndex = nodes[indexForOptionNode].R - 1;
        while (nodes[currentIndex].type == 2) {
            nodes[nodes[currentIndex].U].D = currentIndex;
            nodes[nodes[currentIndex].D].U = currentIndex;
            ++nodes[nodes[currentIndex].L].count; // decrement counter of remaining options covering item
            --currentIndex; // proceed to the next node belonging to the current option.
        }
        nodes[nodes[indexForOptionNode].U].D = indexForOptionNode;
        nodes[nodes[indexForOptionNode].D].U = indexForOptionNode;
    }

    void cover(std::vector<ENode>& nodes, size_t indexForItemNode, bool removeEncounteredOptionNodes) {
        size_t currentIndex = nodes[indexForItemNode].D;
        while (currentIndex != indexForItemNode) {
            hideOption(nodes, currentIndex, removeEncounteredOptionNodes);
            currentIndex = nodes[currentIndex].D;
        }
        nodes[nodes[indexForItemNode].L].R = nodes[indexForItemNode].R;
        nodes[nodes[indexForItemNode].R].L = nodes[indexForItemNode].L;
    }

    void uncover(std::vector<ENode>& nodes, size_t indexForItemNode, bool restoreEncounteredOptionNodes) {
        nodes[nodes[indexForItemNode].L].R = indexForItemNode;
        nodes[nodes[indexForItemNode].R].L = indexForItemNode;
        size_t currentIndex = nodes[indexForItemNode].U;
        while (currentIndex != indexForItemNode) {
            unhideOption(nodes, currentIndex, restoreEncounteredOptionNodes);
            currentIndex = nodes[currentIndex].U;
        }
    }

    void algorithmXHelper(std::vector<ENode>& nodes, int level, std::vector<int>& partialSolution, std::vector<std::vector<int>>& solutions) {
        std::cout << "algoX\n";
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
            if (flag || nodes[currentNodeIndex].count < minOptionsCovering) {
                flag = false;
                minOptionsCovering = nodes[currentNodeIndex].count;
                indexOfChosenItem = currentNodeIndex;
            }
            currentNodeIndex = nodes[currentNodeIndex].R;
        }
        cover(nodes, indexOfChosenItem, false);

        size_t currentOptionNodeIndex = nodes[indexOfChosenItem].D;
        while (currentOptionNodeIndex != indexOfChosenItem) {
            //std::cout << "covering item " << indexOfChosenItem << " with option " << nodes[currentOptionNodeIndex].L << "\n";
            // now cover all the items covered by the current option
            size_t curr = currentOptionNodeIndex + 1;
            while (curr != currentOptionNodeIndex) {
                size_t top = nodes[curr].L;
                if (nodes[curr].type == 1) { // option node
                    curr = top + 1;
                } else {
                    cover(nodes, top, false);
                    ++curr;
                }
            }
            partialSolution[level] = nodes[currentOptionNodeIndex].L; // recall that on a non header node, the L property is used to point to the associated option.
            algorithmXHelper(nodes, level + 1, partialSolution, solutions);
            //std::cout << "uncovering\n";
            // undo the covering
            curr = currentOptionNodeIndex - 1;
            while (curr != currentOptionNodeIndex) {
                size_t top = nodes[curr].R;
                if (nodes[curr].type == 1) { // option node
                    curr = top - 1;
                } else {
                    uncover(nodes, top, false);
                    --curr;
                }
            }
            currentOptionNodeIndex = nodes[currentOptionNodeIndex].D;
        }

        uncover(nodes, indexOfChosenItem, false);
    }

    void increasingOrderHybridSolver_Helper(std::vector<ENode>& nodes, size_t currentOptionNodeIndex, int level, const std::map<size_t, size_t>& optionNodeIndexToOptionIndex, std::vector<int>& partialSolution, std::vector<std::vector<int>>& solutions, size_t sizeToTransitionAt) {
        //printAllNodes(nodes);
        //std::cout << "level: " << level << "\n";
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
            std::vector<size_t> toUnhideStorage;

            while (currentOptionNodeIndex != 0) {
                if (currentOptionNodeIndex > indexOfNodeInFinalPlausibleOption) {
                    break;
                }
                //std::cout << "Selecting option " << optionNodeIndexToOptionIndex.at(currentOptionNodeIndex) << " at level " << level << "\n";
                size_t currIndexWithinOption = currentOptionNodeIndex + 1;

                while (nodes[currIndexWithinOption].type == 2) {
                    //std::cout << "calling cover on index node " << nodes[currIndexWithinOption].L << "\n";
                    cover(nodes, nodes[currIndexWithinOption].L, true);
                    ++currIndexWithinOption;
                }

                size_t next = nodes[0].D;

                partialSolution[level] = optionNodeIndexToOptionIndex.at(currentOptionNodeIndex);
                increasingOrderHybridSolver_Helper(nodes, next, level + 1, optionNodeIndexToOptionIndex, partialSolution, solutions, sizeToTransitionAt);


                --currIndexWithinOption;
                while (nodes[currIndexWithinOption].type == 2) {
                    uncover(nodes, nodes[currIndexWithinOption].L, true);
                    --currIndexWithinOption;
                }

                // TODO: rather than hiding, consider amending the uncovering above to skip this unhiding
                hideOptionUsingOptionNode(nodes, currentOptionNodeIndex);
                /*toUnhideStorage[unhideStorageCurrentIndex] = currentOptionNodeIndex;
                //std::cout << unhideStorageCurrentIndex << "\n";
                ++unhideStorageCurrentIndex;*/
                toUnhideStorage.push_back(currentOptionNodeIndex);

                //std::cout << "trying alternate option of " << nodes[0].D << " at level " << level << "\n";
                currentOptionNodeIndex = nodes[0].D;
            }

            for (auto itr = toUnhideStorage.crbegin(); itr != toUnhideStorage.crend(); ++itr) {
                unhideOptionUsingOptionNode(nodes, *itr);
            }
        } else {
            algorithmXHelper(nodes, level, partialSolution, solutions);
        }
    }
}

std::vector<std::vector<int>> increasingOrderHybridSolver(const std::vector<std::vector<int>>& data, int itemCount, size_t sizeToTransitionAt) {
    std::vector<std::vector<int>> solutions;
    auto [nodes, optionNodeIndexToOptionIndex] = makeLinkStructure<ENode>(data, itemCount);
    std::vector<int> partialSolution(itemCount); // assumes every option covers at least one item.
    //std::vector<size_t> toUnhideStorage(data.size()); // large enough to hold an index for every option.
    increasingOrderHybridSolver_Helper(nodes, nodes[0].D, 0, optionNodeIndexToOptionIndex, partialSolution, solutions, sizeToTransitionAt);
    return solutions;
}