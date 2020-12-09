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

#define ASSERTM_EQ(first, second, message) utest::TestEq(first,second,message,__FILE__, __LINE__, true)
#define ASSERTM_NE(first, second, message) utest::TestNe(first,second,message,__FILE__, __LINE__, true)
#define ASSERTM_LT(first, second, message) utest::TestLt(first,second,message,__FILE__, __LINE__, true)
#define ASSERTM_LE(first, second, message) utest::TestLe(first,second,message,__FILE__, __LINE__, true)
#define ASSERTM_GT(first, second, message) utest::TestGt(first,second,message,__FILE__, __LINE__, true)
#define ASSERTM_GE(first, second, message) utest::TestGe(first,second,message,__FILE__, __LINE__, true)

#define EXPECTM_EQ(first, second, message) utest::TestEq(first,second,message, __FILE__, __LINE__, false)
#define EXPECTM_NE(first, second, message) utest::TestNe(first,second,message, __FILE__, __LINE__, false)
#define EXPECTM_LT(first, second, message) utest::TestLt(first,second,message, __FILE__, __LINE__, false)
#define EXPECTM_LE(first, second, message) utest::TestLe(first,second,message, __FILE__, __LINE__, false)
#define EXPECTM_GT(first, second, message) utest::TestGt(first,second,message, __FILE__, __LINE__, false)
#define EXPECTM_GE(first, second, message) utest::TestGe(first,second,message, __FILE__, __LINE__, false)

#define EXPECT_EQ(first, second) EXPECTM_EQ(first, second, "")
#define EXPECT_NE(first, second) EXPECTM_NE(first, second, "")
#define EXPECT_LT(first, second) EXPECTM_LT(first, second, "")
#define EXPECT_LE(first, second) EXPECTM_LE(first, second, "")
#define EXPECT_GT(first, second) EXPECTM_GT(first, second, "")
#define EXPECT_GE(first, second) EXPECTM_GE(first, second, "")

#define ASSERT_EQ(first, second) ASSERTM_EQ(first, second, "")
#define ASSERT_NE(first, second) ASSERTM_NE(first, second, "")
#define ASSERT_LT(first, second) ASSERTM_LT(first, second, "")
#define ASSERT_LE(first, second) ASSERTM_LE(first, second, "")
#define ASSERT_GT(first, second) ASSERTM_GT(first, second, "")
#define ASSERT_GE(first, second) ASSERTM_GE(first, second, "")

#define ASSERT_BOOL(first, second, cond, msg1, msg2) utest::TestBool(first, second, cond, msg1, msg2 ,__FILE__, __LINE__, true)
#define EXPECT_BOOL(first, second, cond, msg1, msg2) utest::TestBool(first, second, cond, msg1, msg2 ,__FILE__, __LINE__. false)

#define SCOPED_TRACE(message) utest::ScopedTrace(message ,__FILE__, __LINE__)