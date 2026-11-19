#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <optional>
#include <type_traits>
#include "IncreasingOrderShared.h"

template <typename NodeType>
void addToEndOfHorizontalDoublyLinkedList(std::vector<NodeType>& nodeStorage, size_t startIndex, size_t toAddIndex) {
    std::uint32_t endIndex = nodeStorage[startIndex].L;
    nodeStorage[endIndex].R = toAddIndex;
    nodeStorage[startIndex].L = toAddIndex;
    nodeStorage[toAddIndex].L = endIndex;
    nodeStorage[toAddIndex].R = startIndex;
}

template void addToEndOfHorizontalDoublyLinkedList<Node>(std::vector<Node>& nodeStorage, size_t startIndex, size_t toAddIndex);
template void addToEndOfHorizontalDoublyLinkedList<ENode>(std::vector<ENode>& nodeStorage, size_t startIndex, size_t toAddIndex);

template <typename NodeType>
void addToEndOfVerticalDoublyLinkedList(std::vector<NodeType>& nodeStorage, size_t startIndex, size_t toAddIndex) {
    std::uint32_t endIndex = nodeStorage[startIndex].U;
    nodeStorage[endIndex].D = toAddIndex;
    nodeStorage[startIndex].U = toAddIndex;
    nodeStorage[toAddIndex].U = endIndex;
    nodeStorage[toAddIndex].D = startIndex;
}

template void addToEndOfVerticalDoublyLinkedList(std::vector<Node>& nodeStorage, size_t startIndex, size_t toAddIndex);
template void addToEndOfVerticalDoublyLinkedList(std::vector<ENode>& nodeStorage, size_t startIndex, size_t toAddIndex);

template <typename NodeType>
std::pair<std::vector<NodeType>, std::map<size_t, size_t>> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount) {
    int requiredNodeCount = 2 + itemCount;
    for (const std::vector<int>& option: data) {
        requiredNodeCount += (option.size() + 1);
    }
    std::vector<NodeType> nodes;
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

template std::pair<std::vector<Node>, std::map<size_t, size_t>> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount);
template std::pair<std::vector<ENode>, std::map<size_t, size_t>> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount);

template <typename NodeType>
void hideOption(std::vector<NodeType>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide) {
    size_t currentIndex = indexForNormalNode + 1;
    while (currentIndex != indexForNormalNode) {
        if (nodes[currentIndex].type == 1) { // option node
            // Loop back around to the previous option node. i.e. the option node associated with the input normal node.
            currentIndex = nodes[currentIndex].L;
            if (currentIndex > optionNodeToNotHide) {
                // remove option node from vertical list of option nodes
                nodes[nodes[currentIndex].U].D = nodes[currentIndex].D;
                nodes[nodes[currentIndex].D].U = nodes[currentIndex].U;
            }
            ++currentIndex;
        } else {
            nodes[nodes[currentIndex].U].D = nodes[currentIndex].D;
            nodes[nodes[currentIndex].D].U = nodes[currentIndex].U;
            ++currentIndex; // proceed to the next node belonging to the current option.
        }
    }
}

template void hideOption(std::vector<Node>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);
template void hideOption(std::vector<ENode>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);

template <typename NodeType>
void unhideOption(std::vector<NodeType>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide) {
    size_t currentIndex = indexForNormalNode - 1;
    while (currentIndex != indexForNormalNode) {
        if (nodes[currentIndex].type == 1) { // option node
            if (currentIndex > optionNodeToNotHide) {
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

template void unhideOption(std::vector<Node>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);
template void unhideOption(std::vector<ENode>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);

template <typename NodeType>
void cover(std::vector<NodeType>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide) {
    size_t currentIndex = nodes[indexForItemNode].D;
    while (currentIndex != indexForItemNode) {
        hideOption(nodes, currentIndex, optionNodeToNotHide);
        currentIndex = nodes[currentIndex].D;
    }
    nodes[nodes[indexForItemNode].L].R = nodes[indexForItemNode].R;
    nodes[nodes[indexForItemNode].R].L = nodes[indexForItemNode].L;
}

template void cover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);
template void cover(std::vector<ENode>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);

template <typename NodeType>
void uncover(std::vector<NodeType>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide) {
    nodes[nodes[indexForItemNode].L].R = indexForItemNode;
    nodes[nodes[indexForItemNode].R].L = indexForItemNode;
    size_t currentIndex = nodes[indexForItemNode].U;
    while (currentIndex != indexForItemNode) {
        unhideOption(nodes, currentIndex, optionNodeToNotHide);
        currentIndex = nodes[currentIndex].U;
    }
}

template void uncover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);
template void uncover(std::vector<ENode>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);

template <typename NodeType>
std::string nodeToString(const NodeType& node) {
    std::ostringstream ss;
    if (node.type == 0) {
        ss << "header node: { ";
        ss << "U: " << node.U;
        ss << ", D: " << node.D;
        ss << ", L: " << node.L;
        ss << ", R: " << node.R;
        if constexpr (std::is_same_v<NodeType, ENode>) {
            ss << ", count: " << node.top;
        }
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

template std::string nodeToString(const Node& node);
template std::string nodeToString(const ENode& node);

template <typename NodeType>
void printAllNodes(const std::vector<NodeType>& nodes) {
    for (const NodeType& n: nodes) {
        std::cout << nodeToString(n) << "\n";
    }
}

template void printAllNodes(const std::vector<Node>& nodes);
template void printAllNodes(const std::vector<ENode>& nodes);

template <typename NodeType>
std::optional<size_t> getFinalPlausibleIndex(std::vector<NodeType>& nodes) {
    size_t currentNode = nodes[0].R;
    bool flag = true;
    size_t smallestFinalNodeInVerticalList;
    while (currentNode != 0) {
        size_t finalNode = nodes[currentNode].U;
        if (finalNode == currentNode) { // nothing left covering this item
            return std::nullopt;
        }
        if (flag || finalNode < smallestFinalNodeInVerticalList) {
            flag = false;
            smallestFinalNodeInVerticalList = finalNode;
        }
        currentNode = nodes[currentNode].R;
    }
    return std::make_optional(smallestFinalNodeInVerticalList);
}

template std::optional<size_t> getFinalPlausibleIndex(std::vector<Node>& nodes);
template std::optional<size_t> getFinalPlausibleIndex(std::vector<ENode>& nodes);