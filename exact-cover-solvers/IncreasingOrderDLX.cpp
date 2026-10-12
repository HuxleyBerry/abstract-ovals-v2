#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <map>
#include <utility>
#include "IncreasingOrderDLX.h"

namespace {
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

    void addToEndOfHorizontalDoublyLinkedList(std::vector<Node>& nodeStorage, size_t startIndex, size_t toAddIndex) {
        std::uint32_t endIndex = nodeStorage[startIndex].L;
        nodeStorage[endIndex].R = toAddIndex;
        nodeStorage[startIndex].L = toAddIndex;
        nodeStorage[toAddIndex].L = endIndex;
        nodeStorage[toAddIndex].R = startIndex;
    }

    void addToEndOfVerticalDoublyLinkedList(std::vector<Node>& nodeStorage, size_t startIndex, size_t toAddIndex) {
        std::uint32_t endIndex = nodeStorage[startIndex].U;
        nodeStorage[endIndex].D = toAddIndex;
        nodeStorage[startIndex].U = toAddIndex;
        nodeStorage[toAddIndex].U = endIndex;
        nodeStorage[toAddIndex].D = startIndex;
    }

    std::pair<std::vector<Node>, std::map<size_t, size_t>> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount) {
        int requiredNodeCount = 2 + itemCount;
        for (const std::vector<int>& option: data) {
            requiredNodeCount += (option.size() + 1);
        }
        std::vector<Node> nodes;
        nodes.reserve(requiredNodeCount);

        std::map<size_t, size_t> optionNodeIndexToOptionIndex;

        // make root
        nodes.emplace_back(0);
        nodes[0].L = 0;
        nodes[0].R = 0;
        for (size_t i = 1; i <= itemCount; ++i) { // starting at i = 1 to account for the root
            nodes.emplace_back(0);
            nodes[i].U = i;
            nodes[i].D = i;

            addToEndOfHorizontalDoublyLinkedList(nodes, 0, i);
        }
        size_t currentIndex = itemCount + 1;
        for (size_t i = 0; i < data.size(); ++i) {
            const std::vector<int>& option = data[i];
            // add option node.
            nodes.emplace_back(1);
            if (i != 0) {
                nodes[currentIndex].L = currentIndex - data[i - 1].size() - 1;
            }
            nodes[currentIndex].R = currentIndex + option.size() + 1;
            // add to the end of the vertical index down from root
            addToEndOfVerticalDoublyLinkedList(nodes, 0, currentIndex);
            optionNodeIndexToOptionIndex[currentIndex] = i;
            ++currentIndex;
            for (int itemIdx: option) {
                nodes.emplace_back(2);
                addToEndOfVerticalDoublyLinkedList(nodes, itemIdx + 1, currentIndex);
                nodes[currentIndex].L = itemIdx + 1;
                ++currentIndex;
            }
            if (i == data.size() - 1) {
                // Add extra "option node" to the end, but don't link it to the vertical list from root
                nodes.emplace_back(1);
                nodes[currentIndex].L = currentIndex - option.size() - 1;
            }
        }
        return std::pair(nodes, optionNodeIndexToOptionIndex);
    }

    void hideOption(std::vector<Node>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide) {
        //std::cout << "hiding " << indexForNormalNode << "\n";
        size_t currentIndex = indexForNormalNode + 1;
        while (currentIndex != indexForNormalNode) {
            if (nodes[currentIndex].type == 1) { // option node
                // Loop back around to the previous option node. i.e. the option node associated with the input normal node.
                currentIndex = nodes[currentIndex].L;
                if (currentIndex != optionNodeToNotHide) {
                    // remove option node from vertical list of option nodes
                    nodes[nodes[currentIndex].U].D = nodes[currentIndex].D;
                    nodes[nodes[currentIndex].D].U = nodes[currentIndex].U;
                }
                //std::cout << "hide(): removing " << currentIndex << " from vertical list" << "\n";
                ++currentIndex;
            } else {
                nodes[nodes[currentIndex].U].D = nodes[currentIndex].D;
                nodes[nodes[currentIndex].D].U = nodes[currentIndex].U;
                //std::cout << "hide(): removing " << currentIndex << " from vertical list" << "\n";
                ++currentIndex; // proceed to the next node belonging to the current option.
            }
        }
    }

    void unhideOption(std::vector<Node>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide) {
        size_t currentIndex = indexForNormalNode - 1;
        while (currentIndex != indexForNormalNode) {
            if (nodes[currentIndex].type == 1) { // option node
                if (currentIndex != optionNodeToNotHide) {
                    // restore option node into vertical list
                    nodes[nodes[currentIndex].U].D = currentIndex;
                    nodes[nodes[currentIndex].D].U = currentIndex;
                }
                currentIndex = nodes[currentIndex].R - 1; // loop back around to the last node belonging to the current option.
            } else {
                nodes[nodes[currentIndex].U].D = currentIndex;
                nodes[nodes[currentIndex].D].U = currentIndex;
                --currentIndex; // proceed to the previous node belonging to the current option.
            }
        }
    }

    void cover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide) {
        size_t currentIndex = nodes[indexForItemNode].D;
        while (currentIndex != indexForItemNode) {
            //std::cout << "cover(): " << currentIndex << "\n";
            hideOption(nodes, currentIndex, optionNodeToNotHide);
            currentIndex = nodes[currentIndex].D;
        }
        nodes[nodes[indexForItemNode].L].R = nodes[indexForItemNode].R;
        nodes[nodes[indexForItemNode].R].L = nodes[indexForItemNode].L;
    }

    void uncover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide) {
        nodes[nodes[indexForItemNode].L].R = indexForItemNode;
        nodes[nodes[indexForItemNode].R].L = indexForItemNode;
        size_t currentIndex = nodes[indexForItemNode].U;
        while (currentIndex != indexForItemNode) {
            unhideOption(nodes, currentIndex, optionNodeToNotHide);
            currentIndex = nodes[currentIndex].U;
        }
    }

    std::string nodeToString(const Node& node) {
        std::ostringstream ss;
        if (node.type == 0) {
            ss << "header node: { ";
            ss << "U: " << node.U;
            ss << ", D: " << node.D;
            ss << ", L: " << node.L;
            ss << ", R: " << node.R;
            ss << " }";
        } else if (node.type == 1) {
            ss << "option node: { ";
            ss << "U: " << node.U;
            ss << ", D: " << node.D;
            ss << ", L (prev option node): " << node.L;
            ss << ", R (next option node): " << node.R;
            ss << " }";
        } else {
            ss << "normal node: { ";
            ss << "U: " << node.U;
            ss << ", D: " << node.D;
            ss << ", top: " << node.L;
            ss << " }";
        }
        return ss.str();
    }

    void printAllNodes(const std::vector<Node>& nodes) {
        for (const Node& n: nodes) {
            std::cout << nodeToString(n) << "\n";
        }
    }

    void algorithmX_V2_Helper(std::vector<Node>& nodes, size_t currentOptionNodeIndex, int level, const std::map<size_t, size_t>& optionNodeIndexToOptionIndex, std::vector<int>& partialSolution, std::vector<std::vector<int>>& solutions) {
        //printAllNodes(nodes);
        if (nodes[0].R == 0) { // all items covered
            //std::cout << "SOLUTION FOUND\n";
            solutions.emplace_back(partialSolution.cbegin(), partialSolution.cbegin() + level);
            return;
        }
        
        while (currentOptionNodeIndex != 0) {
            //std::cout << "Selecting option " << optionNodeIndexToOptionIndex.at(currentOptionNodeIndex) << " at level " << level << "\n";
            if (optionNodeIndexToOptionIndex.at(currentOptionNodeIndex) == 1 && level == 0) {
                //std::cout << "SPECIAL\n";
                //printAllNodes(nodes);
            }
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