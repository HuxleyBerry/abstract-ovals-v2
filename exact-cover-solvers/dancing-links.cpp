#include "dancing-links.h"
#include <iostream>
#include <limits>
#include <algorithm>

Node* makeLattice(const std::vector<std::vector<int>>& data, int itemCount) {
    Node* root = new Node();
    root->R = root;
    root->L = root;
    root->isHeader = true;
    std::vector<Node*> headers(itemCount, nullptr);
    // set up header nodes -- one for each item
    for (int i = 0; i < itemCount; ++i) {
        Node* headerForItem = new Node();
        headers[i] = headerForItem;
        headerForItem->isHeader = true;
        headerForItem->itemIndex = i;
        headerForItem->optionsCount = 0;
        headerForItem->U = headerForItem;
        headerForItem->D = headerForItem;

        addToEndOfHorizontalDoublyLinkedList(root, headerForItem);
        //std::cout << "header making\n";
        //printNode(*headerForItem);
    }

    for (int i = 0; i < data.size(); ++i) {
        Node* firstNodeForOption = nullptr;
        for (int itemIdx : data[i]) {
            Node* node = new Node();
            node->isHeader = false;
            node->optionIndex = i;
            node->header = headers[itemIdx];
            ++(node->header->optionsCount);
            // add node to vertical list (list corresponding to the item)
            addToEndOfVerticalDoublyLinkedList(node->header, node);

            // add node to horizontal list
            if (firstNodeForOption == nullptr) {
                firstNodeForOption = node;
                node->L = node;
                node->R = node;
            } else {
                addToEndOfHorizontalDoublyLinkedList(firstNodeForOption, node);
            }
        }
    }
    return root;
}

void freeLattice(Node* latticeRoot) {
    Node* currentHeader = latticeRoot->R;
    while (currentHeader != latticeRoot) {
        // free vertical list
        Node* currentInVerticalList = currentHeader->D;
        while (currentInVerticalList != currentHeader) {
            Node* verticalNext = currentInVerticalList->D;
            delete currentInVerticalList;
            currentInVerticalList = verticalNext;
        }

        Node* next = currentHeader->R;
        delete currentHeader;
        currentHeader = next;
    }
    delete latticeRoot;
}

std::vector<std::vector<int>> algorithmX(const std::vector<std::vector<int>>& data, int itemCount) {
    std::vector<std::vector<int>> solutions;

    Node* latticeRoot = makeLattice(data, itemCount);
    std::vector<int> partialSolution(itemCount); // assumes every option covers at least one item.
    algorithmXHelper(latticeRoot, solutions, 0, partialSolution);
    freeLattice(latticeRoot);

    return solutions;
}

void algorithmXHelper(Node* latticeRoot, std::vector<std::vector<int>>& solutions, int optionsSoFarCount, std::vector<int>& partialSolution) {
    if (latticeRoot->R == latticeRoot) {
        solutions.emplace_back(partialSolution.cbegin(), partialSolution.cbegin() + optionsSoFarCount);
        return;
    }
    // find the item covered by the fewest options in order to minimise branching factor
    Node* columnHeader = getColumnIncludedInFewestOptions(latticeRoot);
    coverColumn(columnHeader);
    // loop through all the options covering the item
    Node* currentNodeForItem = columnHeader->D;
    while (currentNodeForItem != columnHeader) {
        Node* currentNodeForOption = currentNodeForItem->R;
        while (currentNodeForOption != currentNodeForItem) {
            coverColumn(currentNodeForOption->header);
            currentNodeForOption = currentNodeForOption->R;
        }
        partialSolution[optionsSoFarCount] = currentNodeForItem->optionIndex;
        algorithmXHelper(latticeRoot, solutions, optionsSoFarCount + 1, partialSolution);
        currentNodeForOption = currentNodeForItem->L;
        while (currentNodeForOption != currentNodeForItem) {
            uncoverColumn(currentNodeForOption->header);
            currentNodeForOption = currentNodeForOption->L;
        }
        currentNodeForItem = currentNodeForItem->D;
    }
    uncoverColumn(columnHeader);
}

Node* getColumnIncludedInFewestOptions(Node* latticeRoot) {
    // assumes some non-root header nodes exist.
    Node* currentHeaderNode = latticeRoot->R;
    Node* columnHeader = currentHeaderNode;
    int minimumFoundOptions = std::numeric_limits<int>::max();
    while (currentHeaderNode != latticeRoot) {
        if (currentHeaderNode->optionsCount < minimumFoundOptions) {
            minimumFoundOptions = currentHeaderNode->optionsCount;
            columnHeader = currentHeaderNode;
        }
        currentHeaderNode = currentHeaderNode->R;
    }
    return columnHeader;
}

void coverColumn(Node* headerNode) {
    // "remove" the header node from the doubly-linked list of header nodes.
    // "removed" is in quotes because it is done in a way which can be reversed.
    headerNode->L->R = headerNode->R;
    headerNode->R->L = headerNode->L;

    Node* currentNodeForItem = headerNode->D;
    while (currentNodeForItem != headerNode){
        Node* currentNodeWithinOption = currentNodeForItem->R;
        while (currentNodeWithinOption != currentNodeForItem) {
            // remove node from vertical list
            currentNodeWithinOption->U->D = currentNodeWithinOption->D;
            currentNodeWithinOption->D->U = currentNodeWithinOption->U;
            // decrement count from header of that vertical list
            --(currentNodeWithinOption->header->optionsCount);

            currentNodeWithinOption = currentNodeWithinOption->R;
        }
        currentNodeForItem = currentNodeForItem->D;
    }
}

void uncoverColumn(Node* headerNode) {
    Node* currentNodeForItem = headerNode->U;
    while (currentNodeForItem != headerNode){
        Node* currentNodeWithinOption = currentNodeForItem->L;
        while (currentNodeWithinOption != currentNodeForItem) {
            // restore node within vertical list
            currentNodeWithinOption->U->D = currentNodeWithinOption;
            currentNodeWithinOption->D->U = currentNodeWithinOption;
            // increment count from header of that vertical list
            ++(currentNodeWithinOption->header->optionsCount);

            currentNodeWithinOption = currentNodeWithinOption->L; // note: traversal in the reverse direction compared to coverColumn().
        }
        currentNodeForItem = currentNodeForItem->U; // note: traversal in the reverse direction compared to coverColumn().
    }

    // undo the earlier removal of the header node from the linked list
    headerNode->L->R = headerNode;
    headerNode->R->L = headerNode;
}

void addToEndOfHorizontalDoublyLinkedList(Node* start, Node* toAdd) {
    Node* end = start->L;
    end->R = toAdd;
    start->L = toAdd;
    toAdd->L = end;
    toAdd->R = start;
}

void addToEndOfVerticalDoublyLinkedList(Node* start, Node* toAdd) {
    Node* end = start->U;
    end->D = toAdd;
    start->U = toAdd;
    toAdd->U = end;
    toAdd->D = start;
}

void printNode(const Node& node) {
    std::cout << "Node with address " << &node << ":\n";
    std::cout << "{\n";
    std::cout << "\tU: " << node.U << "\n";
    std::cout << "\tD: " << node.D << "\n";
    std::cout << "\tL: " << node.L << "\n";
    std::cout << "\tR: " << node.R << "\n";
    std::cout << "\theader: " << node.header << "\n";
    std::cout << "\tisHeader: " << (node.isHeader ? "true" : "false") << "\n";
    std::cout << "}\n";
}

void printNodeExtra(const Node& node) {
    printNode(node);
    std::cout << "node belongs to item " << node.header->itemIndex << " and option " << node.optionIndex << "\n";
}