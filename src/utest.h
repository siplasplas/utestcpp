#pragma once
#include "ConColors.h"
#include <vector>

namespace std {
    inline string to_string(string s){
        return s;
    }
}

namespace utest {

    struct UtestException: public std::exception {
        std::string first;
        std::string message;
        std::string second;
        std::string filename;
        size_t line;
        UtestException(const std::string first,
                              const std::string message,
                              const std::string second,
                              const std::string filename,
                              size_t line):
                first(std::move(first)),second(std::move(second)),
                message(std::move(message)),filename(std::move(filename)),
                line(line) { }

        virtual ~UtestException() throw (){}
        virtual std::string whatstr() const noexcept {
            return first+" " + message + " " + second;
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

    template<typename T>
    void AssertBool(T first, T second, bool cond, const std::string &message, const std::string filename, size_t line)
    {
        std::string firstStr = std::to_string(first);
        std::string secondStr = std::to_string(second);
        if (cond)
            session.assertionsOK++;
        else {
            session.failedAssertions++;
            throw UtestException(firstStr, message, secondStr, filename, line);
        }
    }

    template <typename T>
    void AssertEq(const T& first, const T& second, const std::string filename, size_t line) {
        AssertBool(first, second, first==second, "==", filename, line);
    }

    template <typename T>
    void AssertNeq(T first, T second, const std::string filename, size_t line) {
        AssertBool(first, second, first!=second, "!=", filename, line);
    }

    template <typename T>
    void AssertLt(T first, T second, const std::string filename, size_t line) {
        AssertBool(first, second, first<second, "<", filename, line);
    }

    template <typename T>
    void AssertLe(T first, T second, const std::string filename, size_t line) {
        AssertBool(first, second, first<=second, "<=", filename, line);
    }

    template <typename T>
    void AssertGt(T first, T second, const std::string filename, size_t line) {
        AssertBool(first, second, first>second, ">", filename, line);
    }

    template <typename T>
    void AssertGe(T first, T second, const std::string filename, size_t line) {
        AssertBool(first, second, first>=second, ">=", filename, line);
    }
} // end namespace Catch

#include "macros.h"