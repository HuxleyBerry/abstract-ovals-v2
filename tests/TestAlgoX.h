#include <cassert>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <set>
#include <sstream>
#include "TestSuite.h"
#include "../exact-cover-solvers/dancing-links.h"

std::string vectorToString(std::vector<int> v);

void assertSolutionListCorrect(const std::vector<std::vector<int>>& toTest, const std::vector<std::vector<int>>& expected);

void TestAlgoXV1();

class TestAlgoX : public TestSuite {
public:
    TestAlgoX();
};