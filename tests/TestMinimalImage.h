#include "TestSuite.h"
#include "../orderly/MinimalImage.h"

void TestMinimalImageOfSingleInvolution();
void TestPermutationFinder();
void TestGetInvolutionStabiliser();

class TestMinimalImage : public TestSuite {
public:
    TestMinimalImage() {
        AddToTestSuite(TestPermutationFinder);
        AddToTestSuite(TestMinimalImageOfSingleInvolution);
        AddToTestSuite(TestGetInvolutionStabiliser);
    }
};