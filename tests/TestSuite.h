#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <functional>
#include <vector>
#include <utility>
#include <string>

class TestSuite {
public:
    virtual void setUp();

    virtual void tearDown();

    void runAllTests();

protected:
    void addFunctionToSuite(const std::function<void()>& fun, const char* name);

private:
    std::vector<std::pair<std::function<void()>, std::string>> m_testFunctions;
};

#define AddToTestSuite(funcName) addFunctionToSuite([](){ funcName(); }, #funcName)

#endif