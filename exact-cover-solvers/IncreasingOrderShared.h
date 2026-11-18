#ifndef INCREASING_ORDER_SHARED_H
#define INCREASING_ORDER_SHARED_H

#include <cstdint>
#include <vector>
#include <optional>

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

void uncover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);

std::string nodeToString(const Node& node);

void printAllNodes(const std::vector<Node>& nodes);

std::optional<size_t> getFinalPlausibleIndex(std::vector<Node>& nodes);

#endif