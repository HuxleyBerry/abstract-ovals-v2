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

    bool arePairListsEqual(const std::vector<std::pair<int,int>>& m1, const std::vector<std::pair<int,int>>& m2) {
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

    std::string pairListToString(const std::vector<std::pair<int,int>>& matching) {
        std::ostringstream ss;
        ss << "[";
        for (const auto& [first, second]: matching) {
            ss << "(" << first << ", " << second << "), ";
        }
        ss << "]";
        return ss.str();
    }

    void assertPairListsEqual(const std::vector<std::pair<int,int>>& m1, const std::vector<std::pair<int,int>>& m2) {
        if (!arePairListsEqual(m1, m2)) {
            throw TestException("Matchings not equal. " + pairListToString(m1) + " != " + pairListToString(m2));
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
    assertPermutationsEqual(getMinimalImageOfInvolution(inv1, 0), inv1);

    SymmetricGroupElement inv2 = SymmetricGroupElement<6>("(0,5)(2,4)(1,3)");
    assertPermutationsEqual(getMinimalImageOfInvolution(inv2, 0), inv1);

    SymmetricGroupElement inv3 = SymmetricGroupElement<7>("(3,5)(2,4)(0,6)");
    assertPermutationsEqual(getMinimalImageOfInvolution(inv3, 1), SymmetricGroupElement<7>("(1,2)(3,4)(5,6)"));

    SymmetricGroupElement inv4 = SymmetricGroupElement<8>("(1,5)(3,6)(0,7)");
    assertPermutationsEqual(getMinimalImageOfInvolution(inv4, 2), SymmetricGroupElement<8>("(2,3)(4,5)(6,7)"));
}

void TestGetInvolutionStructure() {
    SymmetricGroupElement inv1 = SymmetricGroupElement<6>("(0,1)(2,3)(4,5)");
    std::vector<std::pair<int,int>> pairs1 = getInvolutionStructure<6>(inv1.getInternalArray()).pairs;
    std::vector<std::pair<int,int>> correctPairs1 = {{0,1},{2,3},{4,5}};
    assertPairListsEqual(pairs1, correctPairs1);

    SymmetricGroupElement inv2 = SymmetricGroupElement<6>("(2,4)(1,5)");
    InvolutionStructure s2 = getInvolutionStructure<6>(inv2.getInternalArray());
    std::vector<std::pair<int,int>> pairs2 = s2.pairs;
    std::vector<std::pair<int,int>> correctPairs2 = {{1,5},{4,2}};
    assertPairListsEqual(pairs2, correctPairs2);

    SymmetricGroupElement inv3 = SymmetricGroupElement<7>("(0,4)(1,5)(2,6)");
    std::vector<std::pair<int,int>> pairs3 = getInvolutionStructure<7>(inv3.getInternalArray()).pairs;
    std::vector<std::pair<int,int>> correctPairs3 = {{1,5},{4,0},{6,2}};
    assertPairListsEqual(pairs3, correctPairs3);
}

void TestGetInvolutionStabiliser() {
    SymmetricGroupElement inv1 = SymmetricGroupElement<6>("(0,1)(2,3)(4,5)");
    SymmetricGroupElement inv2 = SymmetricGroupElement<6>("(2,4)(1,5)");
    SymmetricGroupElement inv3 = SymmetricGroupElement<7>("(0,4)(1,5)(2,6)");
    SymmetricGroupElement inv4 = SymmetricGroupElement<11>("(0,1)(2,3)(4,5)(6,7)(8,9)");

    std::vector<SymmetricGroupElement<6>> stab1 = getInvolutionStabilizer(inv1);
    assertStabilizerValidity(inv1, stab1, 6 * 8);
    std::vector<SymmetricGroupElement<6>> stab2 = getInvolutionStabilizer(inv2);
    assertStabilizerValidity(inv2, stab2, 2 * 4 * 2);
    std::vector<SymmetricGroupElement<7>> stab3 = getInvolutionStabilizer(inv3);
    assertStabilizerValidity(inv3, stab3, 6 * 8);
    std::vector<SymmetricGroupElement<11>> stab4 = getInvolutionStabilizer(inv4);
    assertStabilizerValidity(inv4, stab4, 120 * 32);
}

void TestIsMinimalCheckOfinvolutionSet() {
    std::vector<SymmetricGroupElement<3>> invSet1 = { SymmetricGroupElement<3>("(0,1)") };
    std::vector<SymmetricGroupElement<3>> invSet2 = { SymmetricGroupElement<3>("(1,2)") };
    /*if (isSetOfInvolutionsMinimal(invSet1, 1)) {
        throw TestException("involution set should not be minimal");
    }
    if (!isSetOfInvolutionsMinimal(invSet2, 1)) {
        throw TestException("involution set should be minimal");
    }*/
    std::vector<SymmetricGroupElement<3>> invSet3 = { SymmetricGroupElement<3>("(1,2)"), SymmetricGroupElement<3>("(0,2)") };
    std::vector<SymmetricGroupElement<3>> invSet4 = { SymmetricGroupElement<3>("(1,2)"), SymmetricGroupElement<3>("(0,1)") };
    assertPermutationSetIsOrdered(invSet3);
    assertPermutationSetIsOrdered(invSet4);
    /*if (isSetOfInvolutionsMinimal(invSet3, 1)) {
        throw TestException("involution set" + listOfPermutationsToString(invSet3) + " should not be minimal");
    }*/
    if (!isSetOfInvolutionsMinimal(invSet4, 1)) {
        throw TestException("involution set" + listOfPermutationsToString(invSet4) + " should be minimal");
    }
}