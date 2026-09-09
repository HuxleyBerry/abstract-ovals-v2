#include "AbstractOvalUtils.h"

std::vector<FourPoints> getPointCombinationsForAbstractOval(int order) {
    std::vector<FourPoints> ans;
    for (int i = 0; i <= order; ++i) {
        for (int j = i; j <= order; ++j) {
            for (int k = i + 1; k <= order; ++k) {
                for (int n = k; n <= order; ++n) {
                    // if the order is even, we skip quadruples that would be covered by
                    // the identity permutation
                    if (order%2 == 0 && i == j && k == n) continue;
                    if (k != j && n != j) {
                        ans.emplace_back(i,j,k,n);
                    }
                }
            }
        }
    }
    return ans;
}

int quadrupleToInt(const FourPoints& q, int order) {
    int ans = q.first;
    ans *= (order + 1);
    ans += q.second;
    ans *= (order + 1);
    ans += q.third;
    ans *= (order + 1);
    ans += q.fourth;
    return ans;
}

void printCovering(const std::vector<std::vector<int>>& covering) {
    std::cout << "[\n";
    for (const std::vector<int>& innerVector : covering) {
        std::cout << "\t[ ";
        for (int num: innerVector) {
            std::cout << num << ", ";
        }
        std::cout << "]\n";
    }
    std::cout << "]\n";
}