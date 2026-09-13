#include "TestAbstractOvalFinder.h"

void TestSmallOrders() {
    testAbstractOvalFinderSpecificOrder<4>();
    testAbstractOvalFinderSpecificOrder<5>();
    testAbstractOvalFinderSpecificOrder<7>();
}

TestAbstractOvalFinder::TestAbstractOvalFinder() {
    AddToTestSuite(TestSmallOrders);
}