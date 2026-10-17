#include "TestAbstractOvalFinder.h"

void TestSmallOrders() {
    testBasicAbstractOvalFinderSpecificOrder<4>();
    testBasicAbstractOvalFinderSpecificOrder<5>();
    testBasicAbstractOvalFinderSpecificOrder<7>();
}

void TestSmallOrdersWithOrderly() {
    testOrderlyAbstractOvalFinderSpecificOrder<4>();
    testOrderlyAbstractOvalFinderSpecificOrder<5>();
    testOrderlyAbstractOvalFinderSpecificOrder<7>();
}