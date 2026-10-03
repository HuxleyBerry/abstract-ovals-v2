#include "TestSuite.h"

void TestMinimalImageOfSingleInvolution();
void TestMinimalImageOfSingleInvolutionWithGroupElementFinder();
void TestPermutationFinder();
void TestGetInvolutionStructure();
void TestGetInvolutionStabiliser();
void TestIsMinimalCheckOfinvolutionSetBasicExamples();
void TestIsMinimalCheckOfinvolutionSetMediumExamples();
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
        AddToTestSuite(TestIsMinimalCheckOfinvolutionSetMediumExamples);
        AddToTestSuite(TestIsMinimalCheckOfinvolutionSetMatchesNaiveAlgorithmResult);
    }
};