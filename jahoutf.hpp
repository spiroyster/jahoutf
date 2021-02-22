
#ifndef JAHOUTF_HPP
#define JAHOUTF_HPP

#include <exception>
#include <chrono>
#include <string>
#include <map>
#include <list>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// if c++11
// if c++14
// if gcc
#define JAHOUTF_GCC 1
// ms

#define JAHOUTF_CATCH_EXCEPTION(x) catch (const std::exception& e)\
{\
    jahoutf_current_test_->jahoutf_exception_thrown(#x, e.what());\
}\
catch (...)\
{\
    jahoutf_current_test_->jahoutf_exception_thrown(#x, "Unhandled exception!");\
}

namespace jahoutf
{
    class test;
    class event_interface;
    class report_interface;
    
    namespace _
    {
        class test_result
        {
        public:
            test_result(const std::string& g, const std::string& n) : group(g), name(n) {}
            
            unsigned int total() const { return success.size() + failure.size(); }
            std::string name, group, exception, exception_location;
            std::list<std::string> success, failure;
            unsigned int duration_ms;
        };
        
        class instance
        {
        public:
            ~instance();

            typedef std::map<std::string, std::list<test*>> test_list;
            typedef std::list<test_result> result_list;

            test_list tests_;
            result_list results_;        
            std::shared_ptr<event_interface> event_;
            std::list<std::shared_ptr<report_interface>> reports_;

            bool shuffle_ = false;
        };
    }

    class report_interface
    {
    public:
        virtual void report(const _::instance& session) {}
    };

    class event_interface
    {
    public:
        virtual void case_start(const test& test) {}
        virtual void case_success(const test& test, const std::string& filename, unsigned int lineNum, const std::string& success) {}
        virtual void case_fail(const test& test, const std::string& filename, unsigned int lineNum, const std::string& success) {}
        virtual void case_exception(const test& test, const std::string& exc) {}
        virtual void case_end(const test& test) {}
        virtual void suite_start(const _::instance& session) {}
        virtual void suite_end(const _::instance& session) {}
    };

    extern _::instance& session();

    class test
    {
    public:
        test(const std::string& group, const std::string& name)
        {
            session().results_.push_back(_::test_result(group, name));
            result_ = &session().results_.back();
        }

        virtual void jahoutf_test_invoke()
        {
            jahoutf_current_test_ = this;
            session().event_->case_start(*this);
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            try { jahoutf_test_body(); }
            JAHOUTF_CATCH_EXCEPTION(TestBody)
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            result_->duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(begin - end).count();
            session().event_->case_end(*this);
        }

        virtual void jahoutf_assert_pass(const std::string& file, unsigned int lineNum, const std::string& msg)
        {
            result_->success.push_back(msg);
            session().event_->case_success(*this, file, lineNum, msg);
        }

        virtual void jahoutf_assert_fail(const std::string& file, unsigned int lineNum, const std::string& msg)
        {
            result_->failure.push_back(msg);
            session().event_->case_fail(*this, file, lineNum, msg);
        }

        virtual void jahoutf_exception_thrown(const std::string& location, const std::string& e)
        {
            result_->exception_location = location; result_->exception = e;
            session().event_->case_exception(*this, e);
        }

        const std::string& jahoutf_test_group() const { return result_->group; }
        const std::string& jahoutf_test_name() const { return result_->name; }
        const _::test_result& jahoutf_test_result() const { return *result_; }

        virtual void jahoutf_test_body() = 0;
    protected:
        test* jahoutf_current_test_;

    private:
        _::test_result* result_;

    };

}   // namespace jahoutf

// A test case with just name...
#define TESTCLASSNAME(x) x

// Test case...
#define TEST_CASE_DECLARE(testgroup, testname) namespace testgroup \
{ \
    class testname : public jahoutf::test \
    {    \
    public: \
        testname() : jahoutf::test(#testgroup, #testname) { jahoutf::session().tests_[#testgroup].push_back(this); } \
        void jahoutf_test_body(); \
    }; \
    testname TESTCLASSNAME(testname); } \
void testgroup::testname::jahoutf_test_body()


namespace jahoutf
{

    class fixture
    {
    public:
        test* jahoutf_current_test_;
        virtual void Setup() {}
        virtual void TearDown() {}
    };

}    // namespace jahoutf


// Test fixture...
#define TEST_FIXTURE_DECLARE(testfixture, testgroup, testname) namespace testgroup \
{ \
    class testname : public jahoutf::test \
    { \
    public:\
        class wrapper : public testfixture\
        {\
        public:\
            wrapper(jahoutf::test* t) { jahoutf_current_test_ = t; }\
            void wrapper_test_body();\
        };\
        testname() : jahoutf::test(#testgroup, #testname) { jahoutf::session().tests_[#testgroup].push_back(this); }\
        void jahoutf_test_invoke()\
        {\
            try\
            { \
                wrapper w(this); current_fixture_ = &w;\
                try \
                { \
                    current_fixture_->Setup();\
                    test::jahoutf_test_invoke(); \
                    try { current_fixture_->TearDown(); } \
                    JAHOUTF_CATCH_EXCEPTION(fixture_teardown)\
                }\
                JAHOUTF_CATCH_EXCEPTION(fixture_setup)\
            }\
            JAHOUTF_CATCH_EXCEPTION(fixture_construct)\
        }\
        void jahoutf_test_body() { current_fixture_->wrapper_test_body(); }\
        wrapper* current_fixture_;\
    };\
    testname TESTCLASSNAME(testname); }\
void testgroup::testname::wrapper::wrapper_test_body()


namespace jahoutf
{
    template <class T>
    class param
    {
    public:
        param() {}
        virtual void Setup() {}
        virtual void TearDown() {}
        const T& GetParam() const { return *param_; };
        const T* param_;
        unsigned int param_id_;
    };

    template<class T>
    class values
    {  
        std::vector<T> values_;
    public:
        values(const std::vector<T>& v) : values_(v) {}
        const std::vector<T>& get() const { return values_; }
    };
}

#define TEST_P(testfixture, testname) class testname : public testfixture, public jahoutf::test\
{\
public:\
    testname(jahoutf::test* t, unsigned int n)\
    :   jahoutf::test(t->jahoutf_test_group(), t->jahoutf_test_name() + "[" + std::to_string(n) + "]")\
    {\
        param_id_ = n;\
    }\
    void jahoutf_test_body();\
};\
void testname::jahoutf_test_body()

#define INSTANTIATE_TEST_P(testgroup, testname, testfixture, testvalues) namespace testgroup \
{\
    class testname : public jahoutf::test\
    {\
    public:\
        testname()\
        : jahoutf::test(#testgroup, #testname)\
        {\
            jahoutf::session().tests_[#testgroup].push_back(this);\
        }\
        void jahoutf_test_invoke()\
        {\
            auto params_ = testvalues;\
            for (unsigned int p = 0; p < params_.get().size(); ++p)\
            {\
                try\
                {\
                    testfixture w(this, p);\
                    w.param_ = &params_.get()[p]; \
                    current_fixture_ = &w;\
                    try\
                    { \
                        current_fixture_->Setup();\
                        current_fixture_->jahoutf_test_invoke();\
                        try { current_fixture_->TearDown(); } \
                        JAHOUTF_CATCH_EXCEPTION(fixture_teardown)\
                    }\
                    JAHOUTF_CATCH_EXCEPTION(fixture_setup)\
                }\
                JAHOUTF_CATCH_EXCEPTION(fixture_construct)\
            }\
        }\
        void jahoutf_test_body() {}\
        testfixture* current_fixture_;\
    };\
    testname TESTCLASSNAME(testname);\
}











namespace jahoutf
{
    class section : public test
    {
    public:
        section(const std::string& group, const std::string& name)
        :   test(group, name)
        {
            // test start
            session().event_->case_start(*this);
        }

        ~section()
        {
            // handler for reporting exceptions...
            session().event_->case_end(*this);
        }
        void jahoutf_test_body() {}
    };
}

#define TEST_SECTION_DECLARE(testgroup, testname) if (std::unique_ptr<jahoutf::section> jahoutf_current_test_ = std::make_unique<jahoutf::section>(#testgroup, #testname))










// default xUnit and Console output...
namespace jahoutf
{

    
    class console : public event_interface
    {
        public:
        
        std::string Black(const std::string& msg) { return std::string("\033[1;30m" + msg + "\033[0m"); }
        std::string Red(const std::string& msg) { return std::string("\033[1;31m" + msg + "\033[0m"); }
        std::string Green(const std::string& msg) { return std::string("\033[1;32m" + msg + "\033[0m"); }
        std::string Yellow(const std::string& msg) { return std::string("\033[1;33m" + msg + "\033[0m"); }
        std::string Blue(const std::string& msg) { return std::string("\033[1;34m" + msg + "\033[0m"); }
        std::string Magenta(const std::string& msg) { return std::string("\033[1;35m" + msg + "\033[0m"); }
        std::string Cyan(const std::string& msg) { return std::string("\033[1;36m" + msg + "\033[0m"); }
        std::string White(const std::string& msg) { return std::string("\033[0;37m" + msg + "\033[0m"); }
        std::string Inverse(const std::string& msg) { return std::string("\033[1;7m" + msg + "\033[0m"); }

        std::string header(const test& test) { return std::string("\n" + test.jahoutf_test_group() + "." + test.jahoutf_test_name() + " "); }
        std::string duration(unsigned int d) { return std::string("(" + std::to_string(d) + "ms)"); }
        std::string results_bar(unsigned int successes, unsigned int failures)
        {
            unsigned int total = successes + failures, length = 20;
            float percentage = static_cast<float>(failures) / static_cast<float>(total);
            unsigned int f_bar = static_cast<int>(percentage * static_cast<float>(length));
            if (!f_bar && failures) { f_bar = 1; }
            unsigned int s_bar = length - f_bar;
            std::ostringstream oss;
            oss << "|" << Green(std::string(s_bar, '=')) << Red(std::string(f_bar, '=')) << "| ";
            return oss.str();
        }
        virtual void case_success(const test& test, const std::string& filename, unsigned int lineNum, const std::string& msg) { if (!msg.empty()) { std::cout << header(test) << Cyan(filename + ":") << Yellow(std::to_string(lineNum)) << msg << "\n"; } }
        virtual void case_fail(const test& test, const std::string& filename, unsigned int lineNum, const std::string& msg) { std::cout << header(test) << Red("fail ") << Cyan(filename + ":") << Yellow(std::to_string(lineNum)); if (!msg.empty()) { std::cout << msg << "\n"; } }
        virtual void case_exception(const test& test, const std::string& exc)  { std::cout << header(test) << Magenta("EXCEPTION ") << "thrown in " << test.jahoutf_test_result().exception_location << "\n" << exc; }
        virtual void case_end(const test& test) 
        {
            std::cout << header(test);
            if (test.jahoutf_test_result().failure.empty())
                std::cout << Green(" PASSED ") << std::to_string(test.jahoutf_test_result().success.size()) << "/" << std::to_string(test.jahoutf_test_result().total()) << " assertions succeeded. " << duration(test.jahoutf_test_result().duration_ms) << '\n';
            else
                std::cout << Red(" FAILED ") << std::to_string(test.jahoutf_test_result().failure.size()) << "/" << std::to_string(test.jahoutf_test_result().total()) << " assertions failed.\n";
        }
        virtual void suite_start(const _::instance& session) 
        {
            unsigned int test_count = 0, group_count = 0;
            for (auto g = session.tests_.begin(); g != session.tests_.end(); ++g, ++group_count)
                test_count += g->second.size();
            if (session.shuffle_)
                std::cout << Cyan("[SHUFFLE] ");
            std::cout << Inverse("Running " + std::to_string(test_count) + " tests (" + std::to_string(group_count) + " groups)") << "\n";
        }
        virtual void suite_end(const _::instance& session) 
        {
            std::cout << "\n" << Inverse("Results");
            struct group_result { unsigned int successful_assertions = 0, failed_assertions = 0, test_passes = 0, test_failures = 0, group_duration_ms = 0; };
            std::map<std::string, group_result> group_results;
            group_result total;
            for (auto r = session.results_.begin(); r != session.results_.end(); ++r)
            {
                group_result& g = group_results[r->group];
                g.group_duration_ms = r->duration_ms;
                g.successful_assertions += r->success.size();
                g.failed_assertions += r->failure.size();
                if (r->failure.empty())
                    ++g.test_passes;
                else
                    ++g.test_failures;
                total.group_duration_ms += g.group_duration_ms;
                total.successful_assertions += g.successful_assertions;
                total.failed_assertions += g.failed_assertions;
                total.test_passes += g.test_passes;
                total.test_failures += g.test_failures;
            }
            for (auto g = group_results.begin(); g != group_results.end(); ++g)
            {
                std::cout << "\n" << results_bar(g->second.test_passes, g->second.test_failures) << " {" << Cyan(g->first) << "} ";
                std::cout << std::to_string(g->second.test_passes) << "/" << std::to_string(g->second.test_passes + g->second.test_failures) << " tests passed ";
                std::cout << "(" << std::to_string(g->second.successful_assertions) << "/" << std::to_string(g->second.successful_assertions + g->second.failed_assertions) << " assertions).";
                std::cout << duration(g->second.group_duration_ms);
            }
            std::cout << "\n" << Inverse("Total");
            std::cout << "\n" << results_bar(total.test_passes, total.test_failures);
            std::cout << std::to_string(total.test_passes) << "/" << std::to_string(total.test_passes + total.test_failures) << " tests passed ";
            std::cout << "(" << std::to_string(total.successful_assertions) << "/" << std::to_string(total.successful_assertions + total.failed_assertions) << " assertions).";
            std::cout << duration(total.group_duration_ms) << "\n";
        }
    };

    class xUnit : public jahoutf::report_interface
    {
    public:
        std::string filepath_;
        xUnit(const std::string& filename) : filepath_(filename) {}

        void report(const _::instance& session)
        {
            std::ostringstream oss;
            oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            //oss << "<testsuite name=\"" << session EWDesignDay2020BehaviourTest" tests="2" failures="0" disabled="0" errors="0" time="0.004">"

            // <testsuite name="EWDesignDay2020BehaviourTest" tests="2" failures="0" disabled="0" errors="0" time="0.004">
            //   <testcase name="GetSizingPeriodOptions" status="run" time="0" classname="EWDesignDay2020BehaviourTest" />
            //   <testcase name = "GetSummertimeTemperatureWithMechVentContinuousTreated19Degrees" status = "run" time = "0.001" classname = "CibseSimpleHeatGainBehaviour2015Test">
            //	   <failure message = "c:\users\cordell\documents\soc2\soc\source\unittests\testenergymodel\heatgain.test.cpp:340&#x0A;The difference between TestHelpers::RoundToTwoDecimalPlaces(summertimeTemperature) and 47.84 is 0.52000000000000313, which exceeds 0.1, where&#x0A;TestHelpers::RoundToTwoDecimalPlaces(summertimeTemperature) evaluates to 47.32,&#x0A;47.84 evaluates to 47.840000000000003, and&#x0A;0.1 evaluates to 0.10000000000000001." type = ""><![CDATA[c:\users\cordell\documents\soc2\soc\source\unittests\testenergymodel\heatgain.test.cpp:340
            //	The difference between TestHelpers::RoundToTwoDecimalPlaces(summertimeTemperature) and 47.84 is 0.52000000000000313, which exceeds 0.1, where
            //	TestHelpers::RoundToTwoDecimalPlaces(summertimeTemperature) evaluates to 47.32,
            //	47.84 evaluates to 47.840000000000003, and
            //	0.1 evaluates to 0.10000000000000001.]]></failure>
            //	</testcase>
        }
    };


    // private stuff...
    namespace _
    {

        static void shuffle(instance::test_list& tests)
        {

        }

        static void run_tests(instance::test_list& tests)
        {  
            session().event_->suite_start(session());
            if (session().shuffle_)
                shuffle(tests);

            unsigned int tests_count = 0, tests_passed = 0, tests_failed = 0;
            for (auto groupItr = tests.begin(); groupItr != tests.end(); ++groupItr)
                for (auto testItr = groupItr->second.begin(); testItr != groupItr->second.end(); ++testItr)
                    (*testItr)->jahoutf_test_invoke();
            
            session().event_->suite_end(session());
        }

        static bool process_arguments(const std::string& arg)
        {
            // check if this is a jahoutf argument...
            if (size_t itr = arg.find("-jahoutf_") != std::string::npos)
            {
                std::string jahoutf_arg = arg.substr(itr+8);
                if (jahoutf_arg == "silent") 
                    jahoutf::session().event_.reset(new event_interface());
                else if (jahoutf_arg == "shuffle") 
                    jahoutf::session().shuffle_ = true;
                else if (size_t filename = jahoutf_arg.find("xUnit=") != std::string::npos)
                    jahoutf::session().reports_.push_back(std::make_shared<jahoutf::xUnit>(jahoutf_arg.substr(filename+4)));
                else if (jahoutf_arg == "run")
                {
                    // process the tests we have to only run them...
                }
                else 
                    std::cerr << "jahoutf argument invalid : " << jahoutf_arg << '\n';
                return true;
            }
            return false;
        }
    }

    



}   // namespace jahoutf

// Macro to initialise jahoutf when using custom main entry func... only needs to be used if not using JAHOUTF_MAIN and instead uses own custom main function.
//  jahoutf_XXX arguments not supported with 
#define JAHOUTF_INIT namespace jahoutf \
{ \
    namespace _ \
    { \
        static std::unique_ptr<instance> session_; \
        instance::~instance()\
        {\
            for (auto reportItr = reports_.begin(); reportItr != reports_.end(); ++reportItr)\
            {\
                try { (*reportItr)->report(*this); }\
                catch (...) {}\
            }\
        }\
    } \
    _::instance& session() \
    { \
        if (!_::session_) \
        { \
            _::session_.reset(new _::instance()); \
            _::session_->event_.reset(new console()); \
        } \
        return *_::session_; \
    } \
}

// Macro for defining main function, user can perform custom global setup and teardown (applied to entire test program) also processed -jahoutf arguments.
// Must explicitly call RUNALL, RUNALL_RANDOMIZED or explicit tests?
#define JAHOUTF_MAIN JAHOUTF_INIT \
void main_impl(const std::vector<std::string>& args); \
int main(int argc, char** argv) \
{ \
    std::vector<std::string> args; \
    for (unsigned int c = 0; c < argc; ++c) \
    { \
        std::string arg(*(argv+c)); \
        if (!jahoutf::_::process_arguments(arg)) \
            args.push_back(arg); \
    } \
    main_impl(args); \
    return 0; \
} \
void main_impl(const std::vector<std::string>& args)

// Run all the tests.... used if user has global startup and teardown functionality in main....
#define RUNALL jahoutf::_::run_tests(jahoutf::session().tests_);
// Shuffle all the tests before running them (not applicable to section/inline tests)...
#define SHUFFLE jahoutf::session().shuffle_ = true; RUNALL
// Silence all the events
#define SILENT jahoutf::session().event_.reset(new jahoutf::_::instance::event_interface());
// Maybe post? Post current results to report...



#define TEST_CASE_1(testgroup, testname) TEST_CASE_DECLARE(testgroup, testname)
#define TEST_CASE_2(testname) TEST_CASE_DECLARE(, testname)
#define GET_TEST_CASE_MACRO(_1,_2,TCNAME,...) TCNAME
#define TEST(...) GET_TEST_CASE_MACRO(__VA_ARGS__, TEST_CASE_1, TEST_CASE_2)(__VA_ARGS__)

#define TEST_SECTION_1(testgroup, testname) TEST_SECTION_DECLARE(testgroup, testname)
#define TEST_SECTION_2(testname) TEST_SECTION_DECLARE(, testname)
#define GET_TEST_SECTION_MACRO(_1,_2,TSNAME,...) TSNAME
#define TEST_SECTION(...) GET_TEST_SECTION_MACRO(__VA_ARGS__, TEST_SECTION_1, TEST_SECTION_2)(__VA_ARGS__)

#define TEST_FIXTURE_1(testfixture, testgroup, testname) TEST_FIXTURE_DECLARE(testfixture, testgroup, testname)
#define TEST_FIXTURE_2(testfixture, testname) TEST_FIXTURE_DECLARE(testfixture,, testname)
#define GET_TEST_FIXTURE_MACRO(_1,_2,_3, TFNAME,...) TFNAME
#define TEST_F(...) GET_TEST_FIXTURE_MACRO(__VA_ARGS__, TEST_FIXTURE_1, TEST_FIXTURE_2)(__VA_ARGS__)

// Parameterised tests...


// (Section) Inline tests...


// Assertions...
#define SUCCESS jahoutf_current_test_->jahoutf_assert_pass(__FILE__, __LINE__, "");
#define FAIL jahoutf_current_test_->jahoutf_assert_fail(__FILE__, __LINE__, "");
#define EXPECT_EQ(a, b) if (a == b) { SUCCESS } else { FAIL };
#define EXPECT(a) EXPECT_EQ(a, true);
#define EXPECT_NEAR(a, b, e) if (abs(b-a) <= e) { SUCCESS } else { FAIL };

// EXPECT_THROW
// ASSERT_THAT

#endif  // namespace JAHOUTF_HPP