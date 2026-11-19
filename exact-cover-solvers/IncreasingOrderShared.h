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

struct ENode: public Node {
    // In a header node, used to count the number of remaining options covering the item.
    std::uint32_t top;

    using Node::Node;
};

template <typename NodeType>
void addToEndOfHorizontalDoublyLinkedList(std::vector<NodeType>& nodeStorage, size_t startIndex, size_t toAddIndex);

extern template void addToEndOfHorizontalDoublyLinkedList<Node>(std::vector<Node>& nodeStorage, size_t startIndex, size_t toAddIndex);
extern template void addToEndOfHorizontalDoublyLinkedList<ENode>(std::vector<ENode>& nodeStorage, size_t startIndex, size_t toAddIndex);

template <typename NodeType>
void addToEndOfVerticalDoublyLinkedList(std::vector<NodeType>& nodeStorage, size_t startIndex, size_t toAddIndex);

extern template void addToEndOfVerticalDoublyLinkedList(std::vector<Node>& nodeStorage, size_t startIndex, size_t toAddIndex);
extern template void addToEndOfVerticalDoublyLinkedList(std::vector<ENode>& nodeStorage, size_t startIndex, size_t toAddIndex);

template <typename NodeType>
std::pair<std::vector<NodeType>, std::map<size_t, size_t>> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount);

extern template std::pair<std::vector<Node>, std::map<size_t, size_t>> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount);
extern template std::pair<std::vector<ENode>, std::map<size_t, size_t>> makeLinkStructure(const std::vector<std::vector<int>>& data, int itemCount);

template <typename NodeType>
void hideOption(std::vector<NodeType>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);

extern template void hideOption(std::vector<Node>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);
extern template void hideOption(std::vector<ENode>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);

template <typename NodeType>
void unhideOption(std::vector<NodeType>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);

extern template void unhideOption(std::vector<Node>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);
extern template void unhideOption(std::vector<ENode>& nodes, size_t indexForNormalNode, size_t optionNodeToNotHide);

template <typename NodeType>
void cover(std::vector<NodeType>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);

extern template void cover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);
extern template void cover(std::vector<ENode>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);

template <typename NodeType>
void uncover(std::vector<NodeType>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);

extern template void uncover(std::vector<Node>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);
extern template void uncover(std::vector<ENode>& nodes, size_t indexForItemNode, size_t optionNodeToNotHide);

template <typename NodeType>
std::string nodeToString(const NodeType& node);

extern template std::string nodeToString(const Node& node);
extern template std::string nodeToString(const ENode& node);

template <typename NodeType>
void printAllNodes(const std::vector<NodeType>& nodes);

extern template void printAllNodes(const std::vector<Node>& nodes);
extern template void printAllNodes(const std::vector<ENode>& nodes);

template <typename NodeType>
std::optional<size_t> getFinalPlausibleIndex(std::vector<NodeType>& nodes);

extern template std::optional<size_t> getFinalPlausibleIndex(std::vector<Node>& nodes);
extern template std::optional<size_t> getFinalPlausibleIndex(std::vector<ENode>& nodes);

#endif