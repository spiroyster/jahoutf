#ifndef JAHOUTF_HPP
#define JAHOUTF_HPP

#include <algorithm>
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
            test_result(const std::string& g, const std::string& n) : group(g), name(n), duration_ms(0) {}
            unsigned int total() const { return success.size() + failure.size(); }
            std::string name, group, exception, exception_location;
            std::list<std::string> success, failure;
            unsigned int duration_ms;
        };
        
        class instance
        {
        public:
            typedef std::map<std::string, std::list<test*>> test_list;
            typedef std::list<test_result> result_list;

            test_list test_runner_;
            std::list<std::string> test_runner_patterns_;
            result_list results_;        
            std::shared_ptr<event_interface> event_;
            std::list<std::shared_ptr<report_interface>> reports_;

            bool shuffle_ = false;
            bool list_ = false;
            std::string suite_name_;
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
        virtual void message(const std::string& msg) {}
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
        test(const std::string& group, const std::string& name) : group_(group), name_(name), result_(0)
        {
            jahoutf_current_test_ = this;
        }

        virtual void jahoutf_test_invoke()
        {
            test_result_install();
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
            if (result_)
                result_->failure.push_back(e);
            session().event_->case_exception(*this, e);
        }

        const std::string& jahoutf_test_group() const { return group_; }
        const std::string& jahoutf_test_name() const { return name_; }
        const _::test_result& jahoutf_test_result() const { return *result_; }
        bool jahoutf_test_disabled() const { return disabled_; }
        void jahoutf_test_disabled(bool disabled) { disabled_ = disabled; }

        virtual void jahoutf_test_body() = 0;
    protected:
        test* jahoutf_current_test_;
        bool disabled_ = false;

        void test_runner_install()
        {
            session().test_runner_[group_].push_back(this);
        }
        void test_result_install()
        {
            if (!result_)
            {
                session().results_.push_back(_::test_result(group_, name_));
                result_ = &session().results_.back();
            }
        }
        

        _::test_result* result_;
    private:
        std::string group_, name_;
    };


    class fixture
    {
    public:
        virtual void Setup() {}
        virtual void TearDown() {}
        virtual void prep(const void* param_value) {}
    };

    template<class T>
    class values
    {  
        std::vector<T> values_;
    public:
        values(const std::vector<T>& v) : values_(v) {}
        const std::vector<T>& get_values() const { return values_; }
    };

    template <class T>
    class fixture_param : public fixture
    {
    public:
        const T* param_value_ = 0;
        void prep(const void* p)
        {
            param_value_ = reinterpret_cast<const T*>(p);
        }
        const T& jahoutf_param() 
        {
            if (param_value_)
                return *param_value_; 
            throw std::runtime_error("Test does not support parameters."); 
        }
    };

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


}   // namespace jahoutf

// A test case with just name...
#define JAHOUTF_TESTCLASSNAME(x) x

// Test case...
#define JAHOUTF_TEST_CASE_DEFINE(testgroup, testname) namespace testgroup \
{ \
    class testname : public jahoutf::test \
    {    \
    public: \
        testname() : jahoutf::test(#testgroup, #testname) { test_runner_install(); } \
        void jahoutf_test_body(); \
    }; \
    testname JAHOUTF_TESTCLASSNAME(testname); } \
void testgroup::testname::jahoutf_test_body()

// Test fixture...
#define JAHOUTF_TEST_FIXTURE_DEFINE(testfixture, testgroup, testname) namespace testgroup \
{ \
    class testname : public jahoutf::test \
    { \
    public:\
        class wrapper : public testfixture, public jahoutf::test\
        {\
        public:\
            wrapper() : jahoutf::test(#testgroup, #testname) {}\
            void jahoutf_test_body();\
        };\
        testname() : jahoutf::test(#testgroup, #testname) { test_runner_install(); }\
        void jahoutf_test_invoke()\
        {\
            jahoutf::_::test_result r(jahoutf_test_group(), jahoutf_test_name());\
            result_ = &r;\
            try\
            { \
                wrapper w;\
                try \
                { \
                    w.Setup();\
                    w.jahoutf_test_invoke();\
                    try { w.TearDown(); } \
                    JAHOUTF_CATCH_EXCEPTION(fixture_teardown)\
                }\
                JAHOUTF_CATCH_EXCEPTION(fixture_setup)\
            }\
            JAHOUTF_CATCH_EXCEPTION(fixture_construct)\
        }\
        void jahoutf_test_body() {}\
    };\
    testname JAHOUTF_TESTCLASSNAME(testname); }\
void testgroup::testname::wrapper::jahoutf_test_body()

#define JAHOUTF_TESTPARAMSNAME(testgroup, testname) testname ## testgroup ## _

// Test case values...
#define JAHOUTF_TEST_CASE_VALUES_DEFINE(testgroup, testname, testvalues) namespace testgroup\
{\
    auto JAHOUTF_TESTPARAMSNAME(testgroup, testname) = testvalues;\
    class testname : public jahoutf::test\
    {\
    public:\
        class wrapper : public jahoutf::test\
        {\
        public:\
            wrapper(unsigned int n) : jahoutf::test(#testgroup, #testname + std::string("[") + std::to_string(n) + std::string("]")), param_id_(n) {}\
            void jahoutf_test_body();\
            const auto& jahoutf_value() { return JAHOUTF_TESTPARAMSNAME(testgroup, testname).get_values()[param_id_]; }\
            unsigned int param_id_;\
        };\
        testname() : jahoutf::test(#testgroup, #testname) { test_runner_install(); }\
        void jahoutf_test_invoke()\
        {\
            for (unsigned int p = 0; p < JAHOUTF_TESTPARAMSNAME(testgroup, testname).get_values().size(); ++p)\
            {\
                try\
                {\
                    wrapper w(p);\
                    w.jahoutf_test_invoke();\
                }\
                JAHOUTF_CATCH_EXCEPTION(test_body)\
            }\
        }\
        void jahoutf_test_body() {}\
    };\
    testname JAHOUTF_TESTCLASSNAME(testname);\
}\
void testgroup::testname::wrapper::jahoutf_test_body()

// Test fixture values... TEST_F_VALUES
#define JAHOUTF_TEST_FIXTURE_VALUES_DEFINE(testgroup, testname, testfixture, testvalues) namespace testgroup\
{\
    auto JAHOUTF_TESTPARAMSNAME(testgroup, testname) = testvalues;\
    class testname : public jahoutf::test\
    {\
    public:\
        class wrapper : public jahoutf::test, public testfixture \
        {\
        public:\
            wrapper(unsigned int n) : jahoutf::test(#testgroup, #testname + std::string("[") + std::to_string(n) + std::string("]")), param_id_(n) {}\
            void jahoutf_test_body();\
            const auto& jahoutf_value() { return JAHOUTF_TESTPARAMSNAME(testgroup, testname).get_values()[param_id_]; }\
            unsigned int param_id_;\
        };\
        testname() : jahoutf::test(#testgroup, #testname) { test_runner_install(); }\
        void jahoutf_test_invoke()\
        {\
            for (unsigned int p = 0; p < JAHOUTF_TESTPARAMSNAME(testgroup, testname).get_values().size(); ++p)\
            {\
                try\
                { \
                    wrapper w(p);\
                    w.prep(&JAHOUTF_TESTPARAMSNAME(testgroup, testname).get_values()[p]);\
                    try \
                    { \
                        w.Setup();\
                        w.jahoutf_test_invoke();\
                        try { w.TearDown(); } \
                        JAHOUTF_CATCH_EXCEPTION(fixture_teardown)\
                    }\
                    JAHOUTF_CATCH_EXCEPTION(fixture_setup)\
                }\
                JAHOUTF_CATCH_EXCEPTION(fixture_construct)\
            }\
        }\
        void jahoutf_test_body() {}\
    };\
    testname JAHOUTF_TESTCLASSNAME(testname);\
}\
void testgroup::testname::wrapper::jahoutf_test_body()

#define JAHOUTF_TEST_SECTION_DEFINE(testgroup, testname) if (std::unique_ptr<jahoutf::section> jahoutf_current_test_ = std::make_unique<jahoutf::section>(#testgroup, #testname))









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
        virtual void message(const std::string& msg) { std::cout << msg; }
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
            unsigned int test_count = 0, group_count = 0, disabled_count = 0;
            for (auto g = session.test_runner_.begin(); g != session.test_runner_.end(); ++g, ++group_count)
                test_count += g->second.size();
                
            std::cout << Inverse("Running " + std::to_string(test_count) + " tests (" + std::to_string(group_count) + " groups)") << " (type \"?\" for help)\n";
            if (session.shuffle_)
                std::cout << Cyan("[SHUFFLE] ");
        }
        virtual void suite_end(const _::instance& session) 
        {
            std::cout << "\n" << Inverse("Results");
            struct group_result { unsigned int successful_assertions = 0, failed_assertions = 0, test_passes = 0, test_failures = 0, group_duration_ms = 0; };
            std::map<std::string, group_result> group_results;
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
            }
            group_result total;
            for (auto g = group_results.begin(); g != group_results.end(); ++g)
            {
                std::cout << "\n" << results_bar(g->second.test_passes, g->second.test_failures) << " {" << Cyan(g->first) << "} ";
                std::cout << std::to_string(g->second.test_passes) << "/" << std::to_string(g->second.test_passes + g->second.test_failures) << " tests passed ";
                std::cout << "(" << std::to_string(g->second.successful_assertions) << "/" << std::to_string(g->second.successful_assertions + g->second.failed_assertions) << " assertions).";
                std::cout << duration(g->second.group_duration_ms);
                total.group_duration_ms += g->second.group_duration_ms;
                total.successful_assertions += g->second.successful_assertions;
                total.failed_assertions += g->second.failed_assertions;
                total.test_passes += g->second.test_passes;
                total.test_failures += g->second.test_failures;
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
            //oss << "<testsuite name=\"" << jahoutf::session().suite_name_ << "\" tests=\"" << session(). << "\" failures=\"" << 0 " disabled="0" errors="0" time="0.004">"

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


    // test runner
    namespace _
    {

        static bool test_name_pattern_match(const jahoutf::test& t)
        {
            std::string test_group_and_name(t.jahoutf_test_group() + "." + t.jahoutf_test_name());
            for (auto patternItr = session().test_runner_patterns_.begin(); patternItr != session().test_runner_patterns_.end(); ++patternItr)
            {
                if (*patternItr == test_group_and_name)
                    return true;
            }
            return false;
        }

        static void list_tests(instance::test_list& tests)
        {
            for (auto groupItr = tests.begin(); groupItr != tests.end(); ++groupItr)
                for (auto testItr = groupItr->second.begin(); testItr != groupItr->second.end(); ++testItr)
                    session().event_->message(groupItr->first + "." + (*testItr)->jahoutf_test_name() + "\n");
        }

        static void disable_all_tests(instance::test_list& tests)
        {
            for (auto groupItr = tests.begin(); groupItr != tests.end(); ++groupItr)
                for (auto testItr = groupItr->second.begin(); testItr != groupItr->second.end(); ++testItr)
                    (*testItr)->jahoutf_test_disabled(true);
        }

        static void run_tests(instance::test_list& tests)
        {  
            if (session().list_)
            {
                list_tests(tests);
                return;
            }

            // Process any pattern matching we need to do...
            if (!session().test_runner_patterns_.empty())
            {
                disable_all_tests(tests);
                for (auto groupItr = tests.begin(); groupItr != tests.end(); ++groupItr)
                    for (auto testItr = groupItr->second.begin(); testItr != groupItr->second.end(); ++testItr)
                        if (test_name_pattern_match(**testItr))
                            (*testItr)->jahoutf_test_disabled(false);
            }

            session().event_->suite_start(session());
            unsigned int tests_count = 0, tests_passed = 0, tests_failed = 0;
            for (auto groupItr = tests.begin(); groupItr != tests.end(); ++groupItr)
            {
                //if (session().shuffle_)
                 //   std::random_shuffle(groupItr->second.begin(), groupItr->second.end());
                for (auto testItr = groupItr->second.begin(); testItr != groupItr->second.end(); ++testItr)
                {
                    if (!(*testItr)->jahoutf_test_disabled())
                        (*testItr)->jahoutf_test_invoke();
                }    
            }
                
            session().event_->suite_end(session());
        }

        static std::string help_message()
        {
            std::ostringstream oss;
            oss << "jahoutf test runner.\n";
            oss << "Usage: > namefExe [-silent] [-list] [-shuffle] [-xunit=\"filename.xml\"] [?] test1 test2 ...\n";
            oss << "\n";
            oss << "-list    : list the tests in this test program.\n";
            oss << "-shuffle : shuffle the tests before running them.\n";
            oss << "-silent  : silence events. No consol output.\n";
            oss << "-xunit   : specify an xml filename to report xUnit to.\n";
            oss << "\n";
            oss << "If one or more tests are specfied to run, all tests except those matching the name patterns will be disabled.\n";
            oss << "To run a single test, use {groupname}.{testname} format (i.e group and name seperated by dot).\n";
            oss << "Wildcards supported e.g.\n";
            oss << "All tests in group:  groupname.*\n";
            oss << "All groups with a specific test name: *.testname\n";
            return oss.str();
        }

        static void process_arguments(const std::vector<std::string>& args)
        {
            for (unsigned int arg = 1; arg < args.size(); ++arg)
            {
                if (args[arg] == "-silent")
                    jahoutf::session().event_.reset(new event_interface());
                else if (args[arg] == "-shuffle")
                    jahoutf::session().shuffle_ = true;
                else if (args[arg] == "-xunit=")
                    jahoutf::session().reports_.push_back(std::make_shared<jahoutf::xUnit>(args[arg].substr(7)));
                else if (args[arg] == "-list")
                    jahoutf::session().list_ = true;
                else if (args[arg] == "?")
                {
                    jahoutf::session().event_->message(help_message());
                    jahoutf::session().test_runner_.clear();
                }
                else
                    jahoutf::session().test_runner_patterns_.push_back(args[arg]);    
            }
        }

        static void post_report(const instance& session)
        {
            for (auto reportItr = session.reports_.begin(); reportItr != session.reports_.end(); ++reportItr)
            {
                try 
                { 
                    (*reportItr)->report(session); 
                }
                catch (...) 
                {

                }
            }
        }
    }

    



}   // namespace jahoutf

// Macro to initialise jahoutf when using custom main entry func... only needs to be used if not using JAHOUTF_MAIN and instead uses own custom main function.
//  jahoutf_XXX arguments not supported with 
#define JAHOUTF_INSTANCE namespace jahoutf \
{ \
    namespace _ \
    { \
        static std::unique_ptr<instance> session_; \
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
#define JAHOUTF_TEST_RUNNER JAHOUTF_INSTANCE \
void test_runner_main(); \
int main(int argc, char** argv) \
{ \
    std::vector<std::string> args; \
    for (unsigned int c = 0; c < argc; ++c) { args.push_back(std::string(*(argv+c))); }\
    jahoutf::_::process_arguments(args);\
    if (!jahoutf::session().test_runner_.empty()) { test_runner_main(); } \
    if (!jahoutf::session().results_.empty()) { jahoutf::_::post_report(jahoutf::session()); }\
    return 0; \
} \
void test_runner_main()

// Run all the tests.... used if user has global startup and teardown functionality in main....
#define RUNALL jahoutf::_::run_tests(jahoutf::session().test_runner_);
// Shuffle all the tests before running them (not applicable to section/inline tests)...
#define SHUFFLE jahoutf::session().shuffle_ = true; RUNALL
// Silence all the events
#define SILENT jahoutf::session().event_.reset(new jahoutf::_::instance::event_interface());
// Maybe post? Post current results to report...
#define REPORT(x) x;
#define EVENT(x) x


#define TEST_CASE_1(testgroup, testname) JAHOUTF_TEST_CASE_DEFINE(testgroup, testname)
#define TEST_CASE_2(testname) JAHOUTF_TEST_CASE_DEFINE(, testname)
#define GET_TEST_CASE_MACRO(_1,_2,TCNAME,...) TCNAME
#define TEST(...) GET_TEST_CASE_MACRO(__VA_ARGS__, TEST_CASE_1, TEST_CASE_2)(__VA_ARGS__)

#define TEST_SECTION_1(testgroup, testname) JAHOUTF_TEST_SECTION_DEFINE(testgroup, testname)
#define TEST_SECTION_2(testname) JAHOUTF_TEST_SECTION_DEFINE(, testname)
#define GET_TEST_SECTION_MACRO(_1,_2,TSNAME,...) TSNAME
#define TEST_S(...) GET_TEST_SECTION_MACRO(__VA_ARGS__, TEST_SECTION_1, TEST_SECTION_2)(__VA_ARGS__)

#define TEST_FIXTURE_1(testgroup, testname, testfixture) JAHOUTF_TEST_FIXTURE_DEFINE(testfixture, testgroup, testname)
#define TEST_FIXTURE_2(testname, testfixture) JAHOUTF_TEST_FIXTURE_DEFINE(testfixture,, testname)
#define GET_TEST_FIXTURE_MACRO(_1,_2,_3, TFNAME,...) TFNAME
#define TEST_F(...) GET_TEST_FIXTURE_MACRO(__VA_ARGS__, TEST_FIXTURE_1, TEST_FIXTURE_2)(__VA_ARGS__)

#define TEST_VALUES_1(testgroup, testname, testvalues) JAHOUTF_TEST_CASE_VALUES_DEFINE(testgroup, testname, testvalues)
#define TEST_VALUES_2(testname, testvalues) JAHOUTF_TEST_CASE_VALUES_DEFINE(,testname, testvalues)
#define GET_TEST_VALUES_MACRO(_1,_2,_3, TVNAME,...) TVNAME
#define TEST_VALUES(...) GET_TEST_VALUES_MACRO(__VA_ARGS__, TEST_VALUES_1, TEST_VALUES_2)(__VA_ARGS__)

#define TEST_F_VALUES_1(testgroup, testname, testfixture, testvalues) JAHOUTF_TEST_FIXTURE_VALUES_DEFINE(testgroup, testname, testfixture, testvalues)
#define TEST_F_VALUES_2(testname, testfixture, testvalues) JAHOUTF_TEST_FIXTURE_VALUES_DEFINE(,testname, testfixture, testvalues)
#define GET_TEST_F_VALUES_MACRO(_1,_2,_3,_4, TFVNAME,...) TFVNAME
#define TEST_F_VALUES(...) GET_TEST_F_VALUES_MACRO(__VA_ARGS__, TEST_F_VALUES_1, TEST_F_VALUES_2)(__VA_ARGS__)


// Assertions...
#define SUCCESS jahoutf_current_test_->jahoutf_assert_pass(__FILE__, __LINE__, "");
#define FAIL jahoutf_current_test_->jahoutf_assert_fail(__FILE__, __LINE__, "");
#define EXPECT_EQ(a, b) if (a == b) { SUCCESS } else { FAIL };
#define EXPECT(a) EXPECT_EQ(a, true);
#define EXPECT_NEAR(a, b, e) if (abs(b-a) <= e) { SUCCESS } else { FAIL };

// EXPECT_THROW
// ASSERT_THAT

#endif // JAHOUTF_HPP