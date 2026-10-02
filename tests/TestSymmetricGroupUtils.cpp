#include "TestSuite.h"
#include "../SymmetricGroupUtils.h"
#include "../SymmetricGroupElement.h"
#include "SymmetricGroupTestUtils.h"

void TestEquality() {
    SymmetricGroupElement<6> s1("(0,1)(3,4,5)");
    SymmetricGroupElement<6> s2("(3,4,5)(1,0)");
    assertPermutationEquality(s1, s2);
}

void TestComparison() {
    SymmetricGroupElement<6> s1("(0,3)(1,2,4)");
    SymmetricGroupElement<6> s2("(1,5)(3,4,2)");
    SymmetricGroupElement<6> s3("(0,5)(3,4,2)");
    assertPermutationOrdering(s1, s3);
    assertPermutationOrdering(s2, s1);
    assertPermutationOrdering(s2, s3);
}

void TestMultiplication() {
    SymmetricGroupElement<3> s1("()");
    SymmetricGroupElement<3> s2("()");
    assertPermutationEquality(s1 * s2, s2);
}

void TestConjugation() {
    SymmetricGroupElement<3> s1("(1,2)");
    SymmetricGroupElement<3> s2("(0,1,2)");
    assertPermutationEquality(congjugate(s1, s2), SymmetricGroupElement<3>("(0,2)"));
}