#include <iostream>
#include <string>
#include "TestSuite.h"
#include "TestAbstractOvalFinder.h"
#include "TestAbstractOvalUtils.h"
#include "TestAlgoX.h"

#define RunTestSuite(SuiteName) { SuiteName s = SuiteName(); runTestSuite(s, #SuiteName); }

void runTestSuite(TestSuite& suite, const char* name) {
        std::cout << "Running " << std::string(name) << " Test Suite\n";
        suite.runAllTests();
}

int main() {
    RunTestSuite(TestAlgoX);
    RunTestSuite(TestAbstractOvalFinder);
    RunTestSuite(TestAbstractOvalUtils);
}