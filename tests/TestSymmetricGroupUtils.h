#include "TestSuite.h"

void TestEquality();

void TestComparison();

void TestMultiplication();

void TestConjugation();

class TestSymmetricGroupUtils : public TestSuite {
public:
    TestSymmetricGroupUtils() {
        AddToTestSuite(TestEquality);
        AddToTestSuite(TestComparison);
        AddToTestSuite(TestMultiplication);
        AddToTestSuite(TestConjugation);
    }
};