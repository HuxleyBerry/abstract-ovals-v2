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