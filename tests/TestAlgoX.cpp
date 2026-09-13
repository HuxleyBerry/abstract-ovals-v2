#include <cassert>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <set>
#include <sstream>
#include "TestAlgoX.h"
#include "../exact-cover-solvers/dancing-links.h"

using namespace std::string_literals;

std::string vectorToString(std::vector<int> v) {
    std::ostringstream ss;
    ss << "[";
    for (int num: v) {
        ss << num << ",";
    }
    ss << "]";
    return ss.str();
}

void assertSolutionListCorrect(const std::vector<std::vector<int>>& toTest, const std::vector<std::vector<int>>& expected) {
    if (toTest.size() != expected.size()) {
        throw std::runtime_error("Did not have the expected solution count of "s + std::to_string(expected.size()));
    }
    //inefficient, but it works
    std::set<std::vector<int>> toTestAsSet(toTest.cbegin(), toTest.cend());
    for (const std::vector<int> sol : expected) {
        if (toTestAsSet.count(sol) == 0) {
            throw std::runtime_error("Solution "s + vectorToString(sol) + " missing.\n");
        }
    }
}

void TestAlgoXV1() {
    std::vector<std::vector<int>> data = {{0, 1}, {2}, {0, 1, 2}};
    std::vector<std::vector<int>> expectedSolutions = {{0, 1}, {2}};
    std::vector<std::vector<int>> solutions = algorithmX(data, 3);
    assertSolutionListCorrect(solutions, expectedSolutions);

    data = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5}
    };
    expectedSolutions = {
        {0, 2, 4}
    };
    solutions = algorithmX(data, 6);
    assertSolutionListCorrect(solutions, expectedSolutions);

    data = {
        {0, 1, 2},
        {3, 4, 5},
        {0, 3},
        {1, 4},
        {2, 5}
    };
    expectedSolutions = {
        {0, 1},
        {2, 3, 4}
    };
    solutions = algorithmX(data, 6);
    assertSolutionListCorrect(solutions, expectedSolutions);

    data = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4}
    };
    expectedSolutions = {};
    solutions = algorithmX(data, 5);
    assertSolutionListCorrect(solutions, expectedSolutions);

    data = {
        {0, 1},
        {2, 3},
        {0, 2},
        {1, 3},
        {0, 3},
        {1, 2},
        {4, 5},
        {6, 7},
        {4, 6},
        {5, 7},
        {4, 7},
        {5, 6}
    };
    expectedSolutions = {
        {0, 1, 6, 7},
        {0, 1, 8, 9},
        {0, 1, 10, 11},

        {2, 3, 6, 7},
        {2, 3, 8, 9},
        {2, 3, 10, 11},

        {4, 5, 6, 7},
        {4, 5, 8, 9},
        {4, 5, 10, 11}
    };
    solutions = algorithmX(data, 8);
    assertSolutionListCorrect(solutions, expectedSolutions);

    data = {
        {0, 3, 6},
        {0, 3},
        {3, 4, 6},
        {2, 4, 5},
        {1, 2, 5, 6},
        {1, 6}
    };

    expectedSolutions = {
        {1, 3, 5}
    };
    solutions = algorithmX(data, 7);
    assertSolutionListCorrect(solutions, expectedSolutions);
}

TestAlgoX::TestAlgoX()  {
    AddToTestSuite(TestAlgoXV1);
}