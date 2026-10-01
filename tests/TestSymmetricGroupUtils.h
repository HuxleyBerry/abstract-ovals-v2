#include "TestSuite.h"

void TestEquality();

void TestComparison();

class TestSymmetricGroupUtils : public TestSuite {
public:
    TestSymmetricGroupUtils() {
        AddToTestSuite(TestEquality);
        AddToTestSuite(TestComparison);
    }
};