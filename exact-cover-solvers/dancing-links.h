#ifndef DANCING_LINKS_H
#define DANCING_LINKS_H

#include <vector>

struct Node {
    Node* U;
    Node* D;
    Node* L;
    Node* R;
    Node* header; // not used by header nodes;
    bool isHeader;
    int optionsCount; // only used by header nodes;
    int optionIndex; // not used by header nodes;
    int itemIndex; // used by header nodes. For debugging.
};

void printNode(const Node& node);

void printNodeExtra(const Node& node);

Node* makeLattice(const std::vector<std::vector<int>>& data, int itemCount);

std::vector<std::vector<int>> algorithmX(const std::vector<std::vector<int>>& data, int itemCount);

void algorithmXHelper(Node* latticeRoot, std::vector<std::vector<int>>& solutions, int optionsSoFarCount, std::vector<int> partialSolution);

Node* getColumnIncludedInFewestOptions(Node* latticeRoot);

// covering an an item i means that all the options which cover i are no longer possible.
// Since i is now "covered" any such option being part of the solution would violate the exact cover constraint.
// Therefore, we remove these options from the lattice.
void coverColumn(Node* headerNode);

// reverses coverColumn
void uncoverColumn(Node* headerNode);

void freeLattice(Node* latticeRoot);

void addToEndOfHorizontalDoublyLinkedList(Node* start, Node* toAdd);

void addToEndOfVerticalDoublyLinkedList(Node* start, Node* toAdd);

// assumes v.size() > 0 and idx < 2 * v.size() (i.e. that doubling the size of the vector will be enough)
void safelyAddToVector(std::vector<int> v, int idx, int num);

#endif