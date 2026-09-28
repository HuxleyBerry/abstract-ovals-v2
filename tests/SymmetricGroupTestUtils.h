#include "../SymmetricGroupElement.h"
#include "TestSuite.h"

#define ASSERT_PERMUTATIONS_EQUAL(left, right) if (left != right) throw TestException(left.toString() + " != " + right.toString())