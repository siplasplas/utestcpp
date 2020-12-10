#pragma once
#include "ConColors.h"
#include <vector>

namespace std {
    inline string to_string(string s){
        return s;
    }

	template <typename T1>
	inline string to_stringVec(const std::vector<T1> &v) {
		  string res = "{";
		  for (int i=0; i<v.size(); i++) {
				if (i>0) res+=",";
				res+=to_string(v[i]);
		  }
		  res += "}";
        return res;
    }
}

namespace utest {

    struct UtestException: public std::exception {
        std::string first;
        std::string msg1;
        std::string msg2;
        std::string second;
        std::string filename;
        size_t line;
        UtestException(const std::string first,
                              const std::string msg1,
                              const std::string msg2,
                              const std::string second,
                              const std::string filename,
                              size_t line):
                first(std::move(first)),second(std::move(second)),
                msg1(std::move(msg1)),msg2(std::move(msg2)),filename(std::move(filename)),
                line(line) { }

        virtual ~UtestException() throw (){}
        virtual std::string whatstr() const noexcept {
            return first+" " + msg1 + " " + second + " " + msg2;
        }
    };

    class UtestError: public std::exception {
        std::string message;
    public:
        explicit UtestError(std::string message):
                message(std::move(message)) { }

        virtual ~UtestError() throw (){}
        virtual const char* what() const noexcept {
            return (message).c_str();
        }
    };

    typedef void(*CasFunc)();

    struct TestCase {
        std::string name;
        std::string filename;
        size_t line;
        CasFunc func;
        TestCase(CasFunc func, std::string name, std::string filename, size_t line) :
            func(func),name(name),filename(filename),line(line){
        }
    };

    class Session {
    public:
        size_t assertionsOK = 0;
        size_t failedAssertions = 0;
        std::vector<TestCase> cases;

        Session(){};
        ~Session(){};

        int w(size_t n) {
            std::string str = std::to_string(n);
            return str.length();
        }

        int max(int a, int b) {
            return a>=b?a:b;
        }

        std::string pad(size_t n, int w) {
            std::string str = std::to_string(n);
            for (int i=str.length(); i<w; i++)
                str = ' '+str;
            return str;
        }
        int run( int argc, char* const argv[] ) {
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            std::cout << argv[0] << " is a UTest v1 host application." << std::endl;
            std::cout << "Run with -? for options" << std::endl;

            int failedCases = 0;
            ConColors cc;
            for (auto tc: cases) {
                try {
                    std::cout << std::endl;
                    std::cout << "-------------------------------------------------------------------------------" << std::endl;
                    std::cout << tc.name << std::endl;
                    std::cout << "-------------------------------------------------------------------------------" << std::endl;
                    std::cout << tc.filename << ":" << tc.line << std::endl;
                    std::cout << "..............................................................................." << std::endl;
                    tc.func();
                }
                catch(UtestException &e) {
                    failedCases++;
                    std::cout << std::endl;
                    std::cout << e.filename << ":" << e.line;
                    cc.setColour(FORE_RED);
                    std::cout << " FAILED:";
                    cc.reset();
                    std::cout << std::endl;
                    cc.setColour(FORE_YELLOW);
                    std::cout << e.whatstr() << std::endl;
                    cc.reset();
                }
                catch(UtestError &e) {
                    throw e;
                }
                catch (...) {
                    failedCases++;
                    cc.setColour(FORE_RED);
                    std::cout << "exception"  << std::endl;
                    cc.reset();
                }
            }
            int w1 = max(w(cases.size()),w(assertionsOK+failedAssertions));
            int w2 = max(w(cases.size()-failedCases),w(assertionsOK));
            int w3 = max(w(failedCases),w(failedAssertions));
            std::cout << std::endl;
            if (failedCases>0)
                cc.setColour(FORE_RED);
            else
                cc.setColour(FORE_GREEN);
            std::cout << "===============================================================================" << std::endl;
            cc.reset();
            std::cout << "test cases: ";
            std::cout << pad(cases.size(),w1);
            std::cout << " | ";
            if (cases.size()-failedCases>0)
                cc.setColour(FORE_GREEN);
            std::cout << pad(cases.size()-failedCases,w2) << " passed";
            cc.reset();
            std::cout << " | ";
            if (failedCases>0)
                cc.setColour(FORE_RED);
            std::cout << pad(failedCases,w3) << " failed" << std::endl;;
            cc.reset();

            std::cout << "assertions: ";
            std::cout << pad(assertionsOK,w1);
            std::cout << " | ";
            if (assertionsOK>0)
                cc.setColour(FORE_GREEN);
            std::cout << pad(assertionsOK,2) << " passed";
            cc.reset();
            std::cout << " | ";
            if (failedAssertions>0)
                cc.setColour(FORE_RED);
            std::cout << pad(failedAssertions,w3) << " failed" << std::endl;;
            cc.reset();

            return failedCases>0?2:0;
        }
    };

    inline Session session;

    struct AutoReg{
        AutoReg(CasFunc testFunc, std::string name, std::string filename, size_t line) {
            session.cases.push_back(TestCase(testFunc, name, filename, line));
        }
    };

    template <template <typename T1> typename T, typename T1>
    void TestBool(const T<T1> &first, const T<T1> &second, bool cond, const std::string &msg1, const std::string &msg2, const std::string filename, size_t line, bool fatal)
    {
		  std::string firstStr;
		  std::string secondStr;

		  if constexpr (std::is_same_v<T<T1>, std::vector<T1>>) {
			   firstStr = std::to_stringVec<T1>(first);
            secondStr = std::to_stringVec<T1>(second);
		  } else {
				firstStr = std::to_string(first);
				secondStr = std::to_string(second);
		  }
        if (cond)
            session.assertionsOK++;
        else {
            session.failedAssertions++;
            if (fatal)
                throw UtestException(firstStr, msg1, msg2, secondStr, filename, line);
        }
    }

    template <typename T,
				  typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    void TestBool(T first, T second, bool cond, const std::string &msg1, const std::string &msg2, const std::string filename, size_t line, bool fatal)
    {
		  std::string firstStr;
		  std::string secondStr;

        firstStr = std::to_string(first);
        secondStr = std::to_string(second);

        if (cond)
            session.assertionsOK++;
        else {
            session.failedAssertions++;
            if (fatal)
                throw UtestException(firstStr, msg1, msg2, secondStr, filename, line);
        }
    }


    template <typename T>
    void TestEq(const T& first, const T& second, const std::string &msg2, const std::string filename, size_t line, bool fatal) {
        TestBool(first, second, first==second, "==", msg2, filename, line, fatal);
    }

    template <typename T>
    void TestNe(T first, T second, const std::string &msg2, const std::string filename, size_t line, bool fatal) {
        TestBool(first, second, first!=second, "!=", msg2, filename, line, fatal);
    }

    template <typename T>
    void TestLt(T first, T second, const std::string &msg2, const std::string filename, size_t line, bool fatal) {
        TestBool(first, second, first<second, "<", msg2, filename, line, fatal);
    }

    template <typename T>
    void TestLe(T first, T second, const std::string &msg2, const std::string filename, size_t line, bool fatal) {
        TestBool(first, second, first<=second, "<=", msg2, filename, line, fatal);
    }

    template <typename T>
    void TestGt(T first, T second, const std::string &msg2, const std::string filename, size_t line, bool fatal) {
        TestBool(first, second, first>second, ">", msg2, filename, line, fatal);
    }

    template <typename T>
    void TestGe(T first, T second, const std::string &msg2, const std::string filename, size_t line, bool fatal) {
        TestBool(first, second, first>=second, ">=", msg2, filename, line, fatal);
    }

    inline void ScopedTrace(const std::string &message, const std::string filename, size_t line) {
        std::cout << filename << ":" << line;
        std::cout << std::endl;
        std::cout << message << std::endl;
    }
} // end namespace Catch

#include "macros.h"