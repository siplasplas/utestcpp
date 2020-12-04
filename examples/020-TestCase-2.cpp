// 020-TestCase-2.cpp
#include <utestcpp/utest.h>

// main() provided by Catch in file 020-TestCase-1.cpp.

static int Factorial( int number ) {
   return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
// return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TEST_CASE( "2: Factorial of 0 is 1 (fail)") {
    ASSERT_EQ( Factorial(0) , 1 );
}

TEST_CASE( "2: Factorials of 1 and higher are computed (pass)") {
    ASSERT_EQ( Factorial(1) , 1 );
    ASSERT_EQ( Factorial(2) , 2 );
    ASSERT_EQ( Factorial(3) , 6 );
    ASSERT_EQ( Factorial(10) , 3628800 );
}
