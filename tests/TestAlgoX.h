#include <vector>
#include <string>
#include "TestSuite.h"

std::string vectorToString(std::vector<int> v);

void assertSolutionListCorrect(const std::vector<std::vector<int>>& toTest, const std::vector<std::vector<int>>& expected);

void TestAlgoXV1();
void TestAlgoXV2();
void TestIncreasingOrderDLX();
void TestIncreasingOrderDLXSolutionsAreInOrder();

class TestAlgoX : public TestSuite {
public:
    TestAlgoX()  {
        AddToTestSuite(TestAlgoXV1);
        AddToTestSuite(TestAlgoXV2);
        AddToTestSuite(TestIncreasingOrderDLX);
        AddToTestSuite(TestIncreasingOrderDLXSolutionsAreInOrder);
    }
};