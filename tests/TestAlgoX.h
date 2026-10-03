#include <vector>
#include <string>
#include "TestSuite.h"

std::string vectorToString(std::vector<int> v);

void assertSolutionListCorrect(const std::vector<std::vector<int>>& toTest, const std::vector<std::vector<int>>& expected);

void TestAlgoXV1();

class TestAlgoX : public TestSuite {
public:
    TestAlgoX()  {
        AddToTestSuite(TestAlgoXV1);
    }
};