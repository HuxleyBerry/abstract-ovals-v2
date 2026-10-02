#include "TestSuite.h"
#include "../orderly/MinimalImage.h"

void TestMinimalImageOfSingleInvolution();
void TestMinimalImageOfSingleInvolutionWithGroupElementFinder();
void TestPermutationFinder();
void TestGetInvolutionStructure();
void TestGetInvolutionStabiliser();
void TestIsMinimalCheckOfinvolutionSetBasicExamples();
void TestIsMinimalCheckOfinvolutionSetMatchesNaiveAlgorithmResult();

class TestMinimalImage : public TestSuite {
public:
    TestMinimalImage() {
        AddToTestSuite(TestPermutationFinder);
        AddToTestSuite(TestMinimalImageOfSingleInvolution);
        AddToTestSuite(TestMinimalImageOfSingleInvolutionWithGroupElementFinder);
        AddToTestSuite(TestGetInvolutionStructure);
        AddToTestSuite(TestGetInvolutionStabiliser);
        AddToTestSuite(TestIsMinimalCheckOfinvolutionSetBasicExamples);
        AddToTestSuite(TestIsMinimalCheckOfinvolutionSetMatchesNaiveAlgorithmResult);
    }
};