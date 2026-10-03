#include "AbstractOvalFinder.h"
#include <iostream>

void printAllAbstractOvalsWithIsomorphsRemoved(int order) {
    if (order < 2 || order > 10) {
        std::cerr << "Order " << order << " is not supported\n";
    } else {
        switch (order) {
        case 2:
            printAllAbstractOvalsWithIsomorphsRemoved<2>();
            break;
        case 3:
            printAllAbstractOvalsWithIsomorphsRemoved<3>();
            break;
        case 4:
            printAllAbstractOvalsWithIsomorphsRemoved<4>();
            break;
        case 5:
            printAllAbstractOvalsWithIsomorphsRemoved<5>();
            break;
        case 6:
            printAllAbstractOvalsWithIsomorphsRemoved<6>();
            break;
        case 7:
            printAllAbstractOvalsWithIsomorphsRemoved<7>();
            break;
        case 8:
            printAllAbstractOvalsWithIsomorphsRemoved<8>();
            break;
        case 9:
            printAllAbstractOvalsWithIsomorphsRemoved<9>();
            break;
        case 10:
            printAllAbstractOvalsWithIsomorphsRemoved<10>();
            break;
        }
    }
}