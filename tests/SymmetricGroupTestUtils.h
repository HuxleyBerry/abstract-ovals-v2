#ifndef SYMMETRIC_GROUP_TEST_UTILS_H
#define SYMMETRIC_GROUP_TEST_UTILS_H

#include "../SymmetricGroupElement.h"
#include <vector>
#include <string>

template <size_t N>
void assertPermutationsEqual(const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right) {
    if (left != right) throw TestException(left.toString() + " != " + right.toString());
}

template <size_t N>
void assertStabilizerValidity(const SymmetricGroupElement<N>& element, std::vector<SymmetricGroupElement<N>>& stab, size_t expectedSize) {
    if (stab.size() != expectedSize) {
        throw TestException("Stabilizer of " + element.toString() + " should have " +  std::to_string(expectedSize) + " elements.");
    }
    for (const auto& perm: stab) {
        if (perm * element != element * perm) {
            throw TestException(perm.toString() + " does not commute with " + element.toString() + ".");
            break;
        }
    }
}

template <size_t N>
void assertPermutationEquality(const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right) {
    if (!(left == right)) {
        throw TestException("Expected " + left.toString() + " to equal " + right.toString());
    }
}

template <size_t N>
void assertPermutationOrdering(const SymmetricGroupElement<N>& lesser, const SymmetricGroupElement<N>& greater) {
    if (!(lesser < greater)) {
        throw TestException("Expected " + lesser.toString() + " to be less than " + greater.toString());
    }
}

template <size_t N>
void assertPermutationSetIsOrdered(const std::vector<SymmetricGroupElement<N>>& perms) {
    for (int i = 0; i < perms.size() - 1; ++i) {
        if (perms[i] >= perms[i+1]) {
            throw TestException("Permutation set is not ordered. Element " + std::to_string(i) + " -- " + perms[i].toString() + " >= Element " + std::to_string(i+1) + " -- " + perms[i+1].toString());
        }
    }
}

#endif