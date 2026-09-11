#include "AbstractOvalFinder.h"
#include <iostream>

void printAllAbstractOvals(int order) {
    if (order < 2 || order > 10) {
        std::cerr << "Order " << order << " is not supported\n";
    } else {
        switch (order) {
        case 2:
            printAllAbstractOvals<2>();
            break;
        case 3:
            printAllAbstractOvals<3>();
            break;
        case 4:
            printAllAbstractOvals<4>();
            break;
        case 5:
            printAllAbstractOvals<5>();
            break;
        case 6:
            printAllAbstractOvals<6>();
            break;
        case 7:
            printAllAbstractOvals<7>();
            break;
        case 8:
            printAllAbstractOvals<8>();
            break;
        case 9:
            printAllAbstractOvals<9>();
            break;
        case 10:
            printAllAbstractOvals<10>();
            break;
        }
    }
}