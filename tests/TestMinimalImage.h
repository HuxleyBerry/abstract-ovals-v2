#include "TestSuite.h"
#include "../orderly/MinimalImage.h"

void TestMinimalImageOfSingleInvolution();
void TestPermutationFinder();
void TestGetInvolutionStructure();
void TestGetInvolutionStabiliser();
void TestIsMinimalCheckOfinvolutionSet();

class TestMinimalImage : public TestSuite {
public:
    TestMinimalImage() {
        AddToTestSuite(TestPermutationFinder);
        AddToTestSuite(TestMinimalImageOfSingleInvolution);
        AddToTestSuite(TestGetInvolutionStructure);
        AddToTestSuite(TestGetInvolutionStabiliser);
        AddToTestSuite(TestIsMinimalCheckOfinvolutionSet);
    }
};