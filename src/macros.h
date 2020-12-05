#define INTERNAL_CATCH_UNIQUE_NAME_LINE2( name, line ) name##line
#define INTERNAL_CATCH_UNIQUE_NAME_LINE( name, line ) INTERNAL_CATCH_UNIQUE_NAME_LINE2( name, line )

#define INTERNAL_CATCH_UNIQUE_NAME( name ) INTERNAL_CATCH_UNIQUE_NAME_LINE( name, __COUNTER__ )

#define INTERNAL_CATCH_TESTCASE2( TestName, ... ) \
        static void TestName();                   \
        static utest::AutoReg INTERNAL_CATCH_UNIQUE_NAME(autoRegistrar)(&TestName,__VA_ARGS__, __FILE__, __LINE__);        \
        static void TestName()

#define INTERNAL_CATCH_TESTCASE( ... ) \
        INTERNAL_CATCH_TESTCASE2( INTERNAL_CATCH_UNIQUE_NAME( _____U_T_E_S_T____ ), __VA_ARGS__ )

#define TEST_CASE( ... ) INTERNAL_CATCH_TESTCASE( __VA_ARGS__ )

#define DEFAULT_MAIN int main( int argc, char* const argv[] ){ return utest::session.run( argc, argv ); }

#define ASSERT_EQ(first, second) utest::AssertEq(first,second,__FILE__, __LINE__)
#define ASSERT_NEQ(first, second) utest::AssertNeq(first,second,__FILE__, __LINE__)
#define ASSERT_LT(first, second) utest::AssertLt(first,second,__FILE__, __LINE__)
#define ASSERT_LE(first, second) utest::AssertLe(first,second,__FILE__, __LINE__)
#define ASSERT_GT(first, second) utest::AssertGt(first,second,__FILE__, __LINE__)
#define ASSERT_GE(first, second) utest::AssertGe(first,second,__FILE__, __LINE__)

#define ASSERT_BOOL(first, second, cond, message) utest::AssertBool(first, second, cond, message ,__FILE__, __LINE__)