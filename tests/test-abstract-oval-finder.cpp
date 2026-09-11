#include <stdexcept>
#include <vector>
#include "../SymmetricGroupElement.h"
#include "../AbstractOvalUtils.h"
#include "../AbstractOvalFinder.h"

template <int order>
void testAbstractOvalFinderSpecificOrder() {
    std::vector<std::vector<SymmetricGroupElement<order + 1>>> ovals = basicAbstractOvalFinder<order>();
    for (const std::vector<SymmetricGroupElement<order + 1>>& oval : ovals) {
        if (!validateAbstractOval<order>(oval)) {
            throw std::runtime_error("Oval is not valid");
        }
    }
}

void TestAbstractOvalFinder() {
    testAbstractOvalFinderSpecificOrder<4>();
    testAbstractOvalFinderSpecificOrder<5>();
    testAbstractOvalFinderSpecificOrder<7>();
}