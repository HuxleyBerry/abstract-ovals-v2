#include "AbstractOvalFinder.h"
#include <cstdlib>

int main(int argc, char** argv) {
    int order;
    if (argc <= 1) {
        std::cout << "No order provided\n";
        std::exit(1);
    }
    if ( int parsedOrder = std::atoi(argv[1]); parsedOrder != 0 ) {
        order = parsedOrder;
    } else {
        std::cout << "Invalid order '" << argv[1] << "' provided\n";
        std::exit(1);
    }
    printAllAbstractOvals(order);
}