#include "Complex.h"
#include <utestcpp/utest.h>

TEST_CASE("Complex") {
    Complex c1(2,3);
    Complex c2(1,3);
    Complex c3(2,3);
    ASSERT_EQ(c1,c3);
    ASSERT_EQ(c1,c2);
}

DEFAULT_MAIN