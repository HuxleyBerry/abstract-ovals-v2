#include <iostream>
#include <vector>
#include "SymmetricGroupElement.h"
#include "SymmetricGroupUtils.h"

struct FourPoints {
    unsigned int first;
    unsigned int second;
    unsigned int third;
    unsigned int fourth;

    FourPoints(unsigned int i, unsigned int j, unsigned int k,unsigned int n): first(i), second(j), third(k), fourth(n) {}
};

std::vector<FourPoints> getPointCombinationsForAbstractOval(unsigned int order) {
    std::vector<FourPoints> ans;
    for (unsigned int i = 0; i <= order; ++i) {
        for (unsigned int j = i; j <= order; ++j) {
            for (unsigned int k = i + 1; k <= order; ++k) {
                for (unsigned int n = k; n <= order; ++n) {
                    if (n != j) {
                        ans.emplace_back(i,j,k,n);
                    }
                }
            }
        }
    }
    return ans;
}

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
    std::cout << listOfPermutationsToString(getConjugacyClass(p6)) << "\n";
}