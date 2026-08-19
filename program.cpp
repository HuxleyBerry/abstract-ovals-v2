#include <iostream>
#include <vector>
#include "SymmetricGroupElement.h"
#include "SymmetricGroupUtils.h"
#include "AbstractOvalUtils.h"

int main() {
    SymmetricGroupElement<11> p1("(1,2)(4,5,6)");
    SymmetricGroupElement<11> p2("(1,4)(5,2,7,6)(10,0,3)");
    SymmetricGroupElement<11> p3("(2,4)");
    SymmetricGroupElement<6> p4("(0,1)(2,3)(4,5)");
    std::cout << (p1 * p2).toString() << "\n";
    std::cout << congjugate(p1, p3).toString() << "\n";
    //std::cout << listOfPermutationsToString(getConjugacyClass(p4)) << "\n";
    SymmetricGroupElement<5> p5("(2,3)");
    SymmetricGroupElement<8> p6("(0,1)(2,3,4,5,6)");
    std::cout << listOfPermutationsToString(getAbstractOvalCandidatePermutations<3>()) << "\n";
    auto covering = getCovering<3>();
    printCovering(covering);
}