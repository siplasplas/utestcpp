#pragma once
#include "ConColors.h"
#include <vector>

namespace Tdd {

    struct TddException: public std::exception {
        std::string first;
        std::string message;
        std::string second;
        std::string filename;
        size_t line;
        TddException(const std::string first,
                              const std::string message,
                              const std::string second,
                              const std::string filename,
                              size_t line):
                first(std::move(first)),second(std::move(second)),
                message(std::move(message)),filename(std::move(filename)),
                line(line) { }

        virtual ~TddException() throw (){}
        virtual std::string whatstr() const noexcept {
            return first+" " + message + " " + second;
        }
    };

    class TddError: public std::exception {
        std::string message;
    public:
        explicit TddError(std::string message):
                message(std::move(message)) { }

        virtual ~TddError() throw (){}
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
        size_t assertionsCount = 0;
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
            std::cout << argv[0] << " is a Tdd v1 host application." << std::endl;
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
                catch(TddException &e) {
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
                catch(TddError &e) {
                    throw e;
                }
                catch (...) {
                    failedCases++;
                    cc.setColour(FORE_RED);
                    std::cout << "exception"  << std::endl;
                    cc.reset();
                }
            }
            int w1 = max(w(cases.size()),w(assertionsCount));
            int w2 = max(w(cases.size()-failedCases),w(assertionsCount-failedCases));
            int w3 = w(failedCases);
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
            std::cout << pad(assertionsCount,w1);
            std::cout << " | ";
            if (assertionsCount-failedCases>0)
                cc.setColour(FORE_GREEN);
            std::cout << pad(assertionsCount-failedCases,2) << " passed";
            cc.reset();
            std::cout << " | ";
            if (failedCases>0)
                cc.setColour(FORE_RED);
            std::cout << pad(failedCases,w3) << " failed" << std::endl;;
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

    inline void AssertBoolStr(const std::string & first, const std::string & second, bool cond, const std::string &message, const std::string filename, size_t line) {
        if (cond)
            session.assertionsCount++;
        else
            throw TddException(first, message, second,filename,line);
    }

    inline void AssertBoolInt(const long int &first, const long int & second, bool cond, const std::string &message, const std::string filename, size_t line) {
        AssertBoolStr(std::to_string(first), std::to_string(second), cond, message, filename, line);
    }

    template<typename T>
    void AssertBool(T first, T second, bool cond, const std::string &message, const std::string filename, size_t line)
    {
        if constexpr (std::is_same_v<T, std::string>) {
            AssertBoolStr(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, const char*>) {
            AssertBoolStr(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, uint8_t>)
        {
            AssertBoolInt(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, int8_t>)
        {
            AssertBoolInt(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, uint16_t>)
        {
            AssertBoolInt(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, int16_t>)
        {
            AssertBoolInt(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            AssertBoolInt(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, int32_t>)
        {
            AssertBoolInt(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, uint64_t>)
        {
            AssertBoolInt(first,second,cond,message, filename, line);
        }
        else if constexpr (std::is_same_v<T, int64_t>)
        {
            AssertBoolInt(first,second,cond,message, filename, line);
        }
        else {
            throw TddError("Not handled Assert type");
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