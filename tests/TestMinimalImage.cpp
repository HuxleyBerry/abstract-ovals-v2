#include "TestMinimalImage.h"
#include "SymmetricGroupTestUtils.h"
#include "../SymmetricGroupUtils.h"
#include <string>
#include <set>
#include <sstream>

namespace {
    int pairToInt(const std::pair<int,int>& p, int base) {
        if (p.first < p.second) {
            return base*p.first + p.second;
        } else {
            return base*p.second + p.first;
        }
    }

    bool areMatchingsEqual(const std::vector<std::pair<int,int>>& m1, const std::vector<std::pair<int,int>>& m2) {
        if (m1.size() != m2.size()) {
            return false;
        }
        constexpr int base = 16;
        std::array<bool, base * base> found;
        found.fill(false);
        for (const auto& pair: m1) {
            found[pairToInt(pair, base)] = true;
        }
        for (const auto& pair: m2) {
            if (!found[pairToInt(pair, base)]) {
                return false;
            }
        }
        return true;
    }

    std::string matchingToString(const std::vector<std::pair<int,int>>& matching) {
        std::ostringstream ss;
        ss << "[";
        for (const auto& [first, second]: matching) {
            ss << "(" << first << ", " << second << "), ";
        }
        ss << "]";
        return ss.str();
    }

    void assertMatchingsEqual(const std::vector<std::pair<int,int>>& m1, const std::vector<std::pair<int,int>>& m2) {
        if (!areMatchingsEqual(m1, m2)) {
            throw TestException("Matchings not equal. " + matchingToString(m1) + " != " + matchingToString(m2));
        }
    }
}

void TestPermutationFinder() {
    std::vector<std::vector<size_t>> allPerms = getAllPermutations(5);
    if (allPerms.size() != 120) {
        throw TestException("Should have 120 permutations. Had " + std::to_string(allPerms.size()));
    }
}

void TestMinimalImageOfSingleInvolution() {
    SymmetricGroupElement inv1 = SymmetricGroupElement<6>("(0,1)(2,3)(4,5)");
    ASSERT_PERMUTATIONS_EQUAL(getMinimalImageOfInvolution(inv1, 0), inv1);

    SymmetricGroupElement inv2 = SymmetricGroupElement<6>("(0,5)(2,4)(1,3)");
    ASSERT_PERMUTATIONS_EQUAL(getMinimalImageOfInvolution(inv2, 0), inv1);

    SymmetricGroupElement inv3 = SymmetricGroupElement<7>("(3,5)(2,4)(0,6)");
    ASSERT_PERMUTATIONS_EQUAL(getMinimalImageOfInvolution(inv3, 1), SymmetricGroupElement<7>("(1,2)(3,4)(5,6)"));

    SymmetricGroupElement inv4 = SymmetricGroupElement<8>("(1,5)(3,6)(0,7)");
    ASSERT_PERMUTATIONS_EQUAL(getMinimalImageOfInvolution(inv4, 2), SymmetricGroupElement<8>("(2,3)(4,5)(6,7)"));
}

void TestGetInvolutionStabiliser() {
    SymmetricGroupElement inv1 = SymmetricGroupElement<6>("(0,1)(2,3)(4,5)");
    std::vector<std::pair<int,int>> matchings1 = getMatchingsFromInvolution<6>(inv1.getInternalArray());
    std::vector<std::pair<int,int>> correctMatchings1 = {{0,1},{2,3},{4,5}};
    assertMatchingsEqual(matchings1, correctMatchings1);

    SymmetricGroupElement inv2 = SymmetricGroupElement<6>("(2,4)(1,5)");
    std::vector<std::pair<int,int>> matchings2 = getMatchingsFromInvolution<6>(inv2.getInternalArray());
    std::vector<std::pair<int,int>> correctMatchings2 = {{1,5},{4,2},{0,3}};
    assertMatchingsEqual(matchings2, correctMatchings2);

    SymmetricGroupElement inv3 = SymmetricGroupElement<7>("(0,4)(1,5)(2,6)");
    std::vector<std::pair<int,int>> matchings3 = getMatchingsFromInvolution<7>(inv3.getInternalArray());
    std::vector<std::pair<int,int>> correctMatchings3 = {{1,5},{4,0},{6,2}};
    assertMatchingsEqual(matchings3, correctMatchings3);

    std::vector<SymmetricGroupElement<6>> stab = getInvolutionStabilizer(inv1);
    std::cout << listOfPermutationsToString(stab) << "\n";
    std::cout << matchingToString(matchings1) << "\n";
    if (stab.size() != 8 * 6) {
        throw TestException("Stabilizer should have 48 elements.");
    }
    for (const auto& perm: stab) {
        if (perm * inv1 != inv1 * perm) {
            throw TestException(perm.toString() + " does not commute with " + inv1.toString() + ".");
            break;
        }
    }
    //std::cout << listOfPermutationsToString(stab) << "\n";
}