#include "TestSuite.h"
#include <iostream>

void TestSuite::setUp() {}

void TestSuite::tearDown() {}

void TestSuite::runAllTests() {
    for (const auto& [ testFunc, name ] : m_testFunctions ) {
        setUp();
        // try catch?
        std::cout << "Running " << name << " Test\n";
        testFunc();
        tearDown();
    }
}

void TestSuite::addFunctionToSuite(const std::function<void()>& fun, const char* name) {
    std::string funcName = std::string(name);
    m_testFunctions.push_back({fun, funcName});
}