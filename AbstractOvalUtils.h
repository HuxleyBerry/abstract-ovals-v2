#ifndef ABSTRACT_OVAL_UTILS_H
#define ABSTRACT_OVAL_UTILS_H

#include "SymmetricGroupElement.h"
#include "SymmetricGroupUtils.h"
#include <vector>
#include <array>
#include <utility>


template <int order>
std::vector<SymmetricGroupElement<order + 1>> getAbstractOvalCandidatePermutations() {
    if (order%2 == 0) {
        // we skip the identity permutation;
        std::array<int, order + 1> permArray;
        permArray[order] = order;
        for (int i = 0; i < order/2; ++i) {
            permArray[2 * i] = 2 * i + 1;
            permArray[2 * i + 1] = 2 * i;
        }
        SymmetricGroupElement<order + 1> representative(std::move(permArray));
        return getConjugacyClass(representative);
    } else {
        std::array<int, order + 1> permArray;
        for (int i = 0; i < (order+1)/2; ++i) {
            permArray[2 * i] = 2 * i + 1;
            permArray[2 * i + 1] = 2 * i;
        }
        SymmetricGroupElement<order + 1> zeroFixedRepresentative(permArray);
        permArray[order] = order;
        permArray[order - 1] = order - 1;
        SymmetricGroupElement<order + 1> twoFixedRepresentative(std::move(permArray));
        std::vector<SymmetricGroupElement<order + 1>> zeroFixedConjugacyClass = getConjugacyClass(zeroFixedRepresentative);
        std::vector<SymmetricGroupElement<order + 1>> twoFixedConjugacyClass = getConjugacyClass(twoFixedRepresentative);
        std::cout << zeroFixedConjugacyClass.size() << " " << twoFixedConjugacyClass.size() << "\n";
        zeroFixedConjugacyClass.reserve(zeroFixedConjugacyClass.size() + twoFixedConjugacyClass.size());
        zeroFixedConjugacyClass.insert(zeroFixedConjugacyClass.end(), std::make_move_iterator(twoFixedConjugacyClass.begin()), std::make_move_iterator(twoFixedConjugacyClass.end()));
        return zeroFixedConjugacyClass;
    }
}

#endif