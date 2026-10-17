#include <vector>
#include <functional>
#include "TestSuite.h"
#include "../SymmetricGroupElement.h"
#include "../AbstractOvalUtils.h"
#include "../BasicAbstractOvalFinder.h"
#include "../orderly-abstract-oval-finders/OrderlyAbstractOvalFinder_V1.h"

template <int order>
void testAbstractOvalFinderSpecificOrder(const std::function<std::vector<std::vector<SymmetricGroupElement<order + 1>>>()>& finderFunc) {
    std::vector<std::vector<SymmetricGroupElement<order + 1>>> ovals = finderFunc();
    for (const std::vector<SymmetricGroupElement<order + 1>>& oval : ovals) {
        if (!validateAbstractOval<order>(oval)) {
            throw TestException("Oval is not valid");
        }
    }
}

template <int order>
void testBasicAbstractOvalFinderSpecificOrder() {
    testAbstractOvalFinderSpecificOrder<order>([]{ return basicAbstractOvalFinder<order>(); });
}

template <int order>
void testOrderlyAbstractOvalFinderSpecificOrder() {
    testAbstractOvalFinderSpecificOrder<order>([]{ return OAOF::orderlyAbstractOvalFinder<order>(); });
}

void TestSmallOrders();
void TestSmallOrdersWithOrderly();

class TestAbstractOvalFinder : public TestSuite {
public:
    TestAbstractOvalFinder() {
        AddToTestSuite(TestSmallOrders);
        AddToTestSuite(TestSmallOrdersWithOrderly);
    }
};