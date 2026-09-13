#include "../SymmetricGroupElement.h"
#include "../AbstractOvalUtils.h"
#include "TestAbstractOvalUtils.h"
#include <stdexcept>

void TestCompatibility() {
    SymmetricGroupElement<6> s1("(0,1)(2,3)(4,5)");
    SymmetricGroupElement<6> s2("(0,1)(2,4)(3,5)");
    if (!arePermutationsCompatible<5>(s1, s2)) {
        throw std::runtime_error("should be compatible");
    }

    SymmetricGroupElement<6> s3("(0,1)(2,3)");
    SymmetricGroupElement<6> s4("(0,2)(1,3)");
    if (arePermutationsCompatible<5>(s3, s4)) {
        throw std::runtime_error("should be incompatible");
    }
}

TestAbstractOvalUtils::TestAbstractOvalUtils() {
    AddToTestSuite(TestCompatibility);
}