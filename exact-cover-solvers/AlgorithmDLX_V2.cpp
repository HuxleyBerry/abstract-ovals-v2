#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include "AlgorithmDLX_V2.h"

namespace {
    struct Node {
        std::uint32_t L = 0; // in non header nodes, this stores the index of the associated option
        std::uint32_t R = 0;
        std::uint32_t U = 0;
        std::uint32_t D = 0;
        std::uint32_t top; // in header nodes, this instead counts the number of options covering the item

        Node(std::uint32_t t): top(t) {}
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

    std::vector<Node> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount) {
        int requiredNodeCount = 2 + itemCount;
        for (const std::vector<int>& option: data) {
            requiredNodeCount += (option.size() + 1);
        }
        std::vector<Node> nodes;
        nodes.reserve(requiredNodeCount);

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
            // add spacer. Spacers use the convention that Node.top = 0
            nodes.emplace_back(0);
            if (i != 0) {
                nodes[currentIndex].U = currentIndex - data[i - 1].size();
            }
            nodes[currentIndex].D = currentIndex + option.size();
            ++currentIndex;
            for (int itemIdx: option) {
                nodes.emplace_back(itemIdx + 1); // +1 because index 0 of nodes is reserved for the root.
                addToEndOfVerticalDoublyLinkedList(nodes, itemIdx + 1, currentIndex);
                nodes[currentIndex].L = i;
                ++nodes[itemIdx + 1].top; // increment the count of options covering the specific item.
                ++currentIndex;
            }
            if (i == data.size() - 1) {
                // add final spacer
                nodes.emplace_back(0);
                nodes[currentIndex].U = currentIndex - option.size();
            }
        }
        return nodes;
    }

    void hideOption(std::vector<Node>& nodes, size_t indexForNodeWithinOption) {
        size_t currentIndex = indexForNodeWithinOption + 1;
        while (currentIndex != indexForNodeWithinOption) {
            size_t top = nodes[currentIndex].top;
            if (top == 0) { // spacer node
                currentIndex = nodes[currentIndex].U; // loop back around to the first node belonging to the current option.
            } else {
                nodes[nodes[currentIndex].U].D = nodes[currentIndex].D;
                nodes[nodes[currentIndex].D].U = nodes[currentIndex].U;
                --nodes[top].top;
                ++currentIndex; // proceed to the next node belonging to the current option.
            }
        }
    }

    void unhideOption(std::vector<Node>& nodes, size_t indexForNodeWithinOption) {
        size_t currentIndex = indexForNodeWithinOption - 1;
        while (currentIndex != indexForNodeWithinOption) {
            size_t top = nodes[currentIndex].top;
            if (top == 0) { // spacer node
                currentIndex = nodes[currentIndex].D; // loop back around to the last node belonging to the current option.
            } else {
                nodes[nodes[currentIndex].U].D = currentIndex;
                nodes[nodes[currentIndex].D].U = currentIndex;
                ++nodes[top].top;
                --currentIndex; // proceed to the previous node belonging to the current option.
            }
        }
    }

    void cover(std::vector<Node>& nodes, size_t indexForItem) {
        size_t currentIndex = nodes[indexForItem].D;
        while (currentIndex != indexForItem) {
            hideOption(nodes, currentIndex);
            currentIndex = nodes[currentIndex].D;
        }
        nodes[nodes[indexForItem].L].R = nodes[indexForItem].R;
        nodes[nodes[indexForItem].R].L = nodes[indexForItem].L;
    }

    void uncover(std::vector<Node>& nodes, size_t indexForItem) {
        nodes[nodes[indexForItem].L].R = indexForItem;
        nodes[nodes[indexForItem].R].L = indexForItem;
        size_t currentIndex = nodes[indexForItem].U;
        while (currentIndex != indexForItem) {
            unhideOption(nodes, currentIndex);
            currentIndex = nodes[currentIndex].U;
        }
    }

    std::string nodeToString(const Node& node) {
        std::ostringstream ss;
        if (node.L != 0 || node.R != 0) {
            ss << "header node: { ";
            ss << "U: " << node.U;
            ss << ", D: " << node.D;
            ss << ", L: " << node.L;
            ss << ", R: " << node.R;
            ss << ", options covering: " << node.top;
            ss << " }";
        } else {
            if (node.top == 0) {
                ss << "spacer node: { ";
                ss << "start of prev opt: " << node.U;
                ss << ", end of next opt: " << node.D;
                ss << " }";
            } else {
                ss << "normal node: { ";
                ss << "U: " << node.U;
                ss << ", D: " << node.D;
                ss << ", top: " << node.top;
                ss << ", option index: " << node.L;
                ss << " }";
            }
        }
        return ss.str();
    }

    void printAllNodes(const std::vector<Node>& nodes) {
        for (const Node& n: nodes) {
            std::cout << nodeToString(n) << "\n";
        }
    }

    void algorithmX_V2_Helper(std::vector<Node>& nodes, int level, std::vector<int>& partialSolution, std::vector<std::vector<int>>& solutions) {
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
            algorithmX_V2_Helper(nodes, level + 1, partialSolution, solutions);
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
}

std::vector<std::vector<int>> algorithmX_V2(const std::vector<std::vector<int>>& data, int itemCount) {
    std::vector<std::vector<int>> solutions;
    std::vector<Node> nodes = makeLinkStructure(data, itemCount);
    std::vector<int> partialSolution(itemCount); // assumes every option covers at least one item.
    algorithmX_V2_Helper(nodes, 0, partialSolution, solutions);
    return solutions;
}