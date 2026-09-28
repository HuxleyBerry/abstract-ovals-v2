#include "TestSuite.h"
#include <iostream>

void TestSuite::setUp() {}

void TestSuite::tearDown() {}

void TestSuite::runAllTests() {
    for (const auto& [ testFunc, name ] : m_testFunctions ) {
        setUp();
        std::cout << "Running " << name << " test\n";
        try {
            testFunc();
        } catch (const TestException& ex) {
            std::cout << "Test \"" << name << "\" failed with the following error:\n\t" << ex.what() << "\n";
        }
        catch (const std::exception& ex) {
            std::cout << "Test \"" << name << "\" failed with an unexpected exception:\n\t" << ex.what() << "\n";
        }
        tearDown();
    }
}

void TestSuite::addFunctionToSuite(const std::function<void()>& fun, const char* name) {
    std::string funcName = std::string(name);
    m_testFunctions.push_back({fun, funcName});
}