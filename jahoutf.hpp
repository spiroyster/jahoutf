
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
#include <vector>

// if c++11
// if c++14
// if gcc
#define JAHOUTF_GCC 1
// ms

#define CATCH(x) catch (const std::exception& e)\
{\
    test_case_report_thrown(#x, e.what());\
}\
catch (...)\
{\
    test_case_report_thrown(#x, "Unhandled exception!");\
}

namespace jahoutf
{
    class test_interface;
    
    namespace _
    {
        class instance
        {
            public:

            bool shuffle_ = false;

            // the tests to run...
            typedef std::map<std::string,std::list<test_interface*>> test_list;
            test_list tests_;        

            // run tests
            void run_tests();

            class event_interface_base
            {
            public:
                
                virtual void case_start(const test_interface& test) {}
                virtual void case_success(const test_interface& test, const std::string& filename, unsigned int lineNum, const std::string& success) {}
                virtual void case_fail(const test_interface& test, const std::string& filename, unsigned int lineNum, const std::string& success) {}
                virtual void case_exception(const test_interface& test, const std::string& exc) {}
                virtual void case_end(const test_interface& test) {}
                virtual void testsuite_start(const instance& session) {}
                virtual void testsuite_end(const instance& session) {}
                
            };

            std::unique_ptr<event_interface_base> event_;

            class report_interface
            {
                public:
                virtual void out(const instance& session) {}
            };

            std::list<std::unique_ptr<report_interface>> reports_;
            
        };
    }

    extern _::instance& session();
    
    class test_interface
    {
    public:

        // invoke test
        test_interface(const std::string& group, const std::string& name )
        :   name_(name), group_(group), duration_ms_(0)
        {
            session().tests_[group_].push_back(this);
        }

        virtual ~test_interface() {}

        virtual void invoke()
        {
            // start timer
            session().event_->case_start(*this);
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            
            try { run(); }
            CATCH(TestBody)

            // stop timer...
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            duration_ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(begin - end);
            session().event_->case_end(*this);
        }
        
        virtual void run() = 0;

        void test_case_report_success(const std::string& file, unsigned int lineNum, const std::string& msg = "")
		{
			successes_.push_back(msg);
			session().event_->case_success(*this, file, lineNum, msg);
		}

        void test_case_report_fail(const std::string& file, unsigned int lineNum, const std::string& msg = "")
		{
			failures_.push_back(msg);
            session().event_->case_fail(*this, file, lineNum, msg);
		}

        void test_case_report_thrown(const std::string& location, const std::string& e)
        {
            exception_ = e;
            exception_location_ = location;
            session().event_->case_exception(*this, e);
        }

        const std::list<std::string>& successes() const { return successes_; }
        const std::list<std::string>& failures() const { return failures_; }
        const std::string& exception() const { return exception_; }
        const std::string& exception_location() const { return exception_location_; }
        const std::string& group() const { return group_; }
        const std::string& name() const { return name_; }
        unsigned int duration_ms() const { return duration_ms_.count(); }

private:
        std::list<std::string> successes_, failures_;
        std::string name_, group_, exception_, exception_location_;
        std::chrono::milliseconds duration_ms_;
    };

    class test_case : public test_interface
    {
        public:
        test_case(const std::string& group, const std::string& name)
        :   test_interface(group, name)
        {
        }

        void invoke()
        {
            test_interface::invoke();
        }
    };

    class test_fixture
    {
        public:
        test_interface* test_;

        virtual void Setup() {}
        virtual void TearDown() {}
        void test_case_report_success(const std::string& file, unsigned int lineNum, const std::string& msg = "") 
        {
            if (test_)
                test_->test_case_report_success(file, lineNum, msg);
        }
        void test_case_report_fail(const std::string& file, unsigned int lineNum, const std::string& msg = "") 
        {
            if (test_)
                test_->test_case_report_fail(file, lineNum, msg);
        }
        void test_case_report_thrown(const std::string& location, const std::string& e) 
        {
            if (test_)
                test_->test_case_report_thrown(location, e);
        }
    };

    template <class T>
    class test_param
    {

    };
    
    
    class test_inline : public test_interface
    {
        // maybe inline test which isn't run by the session, but still output xUnit compatible if desired...

    };


    // private stuff...
    namespace _
    {

        class xUnit : public instance::report_interface
        {
            struct attr
            {
                attr(const std::string& name, const std::string& value)
                :   name_(name), value_(value) {}
                
                std::string name_, value_;
            };

            struct element
            {
                element(const std::string& name, const std::list<attr>& attributes, const std::string& content)
                :   name_(name), attributes_(attributes), content_(content) {}

                std::list<element> elements_;
                std::list<attr> attributes_;
                std::string name_, content_;
            };

            void replace_chars(std::string& str, const std::string& what, const std::string& with)
            {
                std::size_t itr = str.find(what);
                while (itr != std::string::npos)
                {
                    str.replace(itr, itr + what.size(), with.c_str());
                    itr = str.find(what);
                }
            }

            // escape
            std::string escape(const std::string& syntax)
            {
                std::string result = syntax;
                replace_chars(result, ">", "&gt;");
                replace_chars(result, "<", "&lt;");
                replace_chars(result, "&", "&amp;");
                replace_chars(result, "'", "&apos;");
                replace_chars(result, "\"", "&quot;");
                return result;
            }

            // generate_syntax
            std::string out_elements_and_attrs(const element& element, const std::string& indent)
            {
                std::string result = indent + "<" + element.name_ + " ";
                for (auto itr = element.attributes_.begin(); itr != element.attributes_.end(); ++itr)
                    result += itr->name_ + "=\"" + escape(itr->value_) + "\" ";
                if (element.content_.empty() && element.elements_.empty())
                    result += "/>\n";
                else
                {
                    result += ">\n";
                    if (!element.content_.empty())
                        result += indent + escape(element.content_);
                    for (auto itr = element.elements_.begin(); itr != element.elements_.end(); ++itr)
                        result += out_elements_and_attrs(*itr, indent + "    ");
                    result += indent + "</" + element.name_ + ">\n";
                }
                return result;
            }

            element generate_testsuite(const std::pair<std::string,std::list<test_interface*>>& test_group)
            {
                return element("", {}, {});
            }

            std::string generate_syntax(const element& root)
            {
                std::string result = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
                result += out_elements_and_attrs(root, "");
			    return result;
            }

            std::string filepath_;

            public:

            xUnit(const std::string& filepath)
            :   filepath_(filepath)
            {
            }

            void out(const instance& session)
            {
                element root("testsuites",
                {
                    attr("tests", std::to_string(0)),
                    attr("failures", std::to_string(0)),
                    attr("disabled", std::to_string(0)),
                    attr("errors", std::to_string(0)),
                    attr("timestamp", std::to_string(0)),
                    attr("time", std::to_string(0)),
                    attr("now", std::to_string(0))
                }, "");
                for (auto groupItr = session.tests_.begin(); groupItr != session.tests_.end(); ++groupItr)
                    root.elements_.push_back(generate_testsuite(*groupItr));

                std::ofstream file(filepath_.c_str());
                file << generate_syntax(root);
            }
        };

        static void shuffle(instance::test_list& tests)
        {

        }

        static void run_tests(instance::test_list& tests)
        {  
            session().event_->testsuite_start(session());
            if (session().shuffle_)
                shuffle(tests);

            unsigned int tests_count = 0;
            unsigned int tests_passed = 0;
            unsigned int tests_failed = 0;

            for (auto groupItr = tests.begin(); groupItr != tests.end(); ++groupItr)
            {
                for (auto testItr = groupItr->second.begin(); testItr != groupItr->second.end(); ++testItr)
                {
                    (*testItr)->invoke();
                    if ((*testItr)->failures().empty())
                        ++tests_passed;
                    else
                        ++tests_failed;
                }
            }
            session().event_->testsuite_end(session());
            for (auto reportItr = session().reports_.begin(); reportItr != session().reports_.end(); ++reportItr)
                (*reportItr)->out(session());
        }

        static bool process_arguments(const std::string& arg)
        {
            // check if this is a jahoutf argument...
            if (size_t itr = arg.find("-jahoutf_") != std::string::npos)
            {
                std::string jahoutf_arg = arg.substr(itr+8);
                if (jahoutf_arg == "silent") 
                    jahoutf::session().event_.reset(new _::instance::event_interface_base());
                else if (jahoutf_arg == "shuffle") 
                    jahoutf::session().shuffle_ = true;
                else if (size_t filename = jahoutf_arg.find("xUnit=") != std::string::npos)
                    jahoutf::session().reports_.push_back(std::make_unique<_::xUnit>(jahoutf_arg.substr(filename+4)));
                else 
                    std::cerr << "jahoutf argument invalid : " << jahoutf_arg << '\n';
                return true;
            }
            return false;
        }
    }


    class event_interface : public _::instance::event_interface_base
    {
        public:
        enum col { R, G, B, C, Y, M, K, W, I };
        void output(const std::string& msg, const col& c = W) 
        { 
            #ifdef JAHOUTF_GCC
                switch (c)
                {
                    case col::K: std::cout << "\033[1;30m" << msg << "\033[0m"; break;
                    case col::R: std::cout << "\033[1;31m" << msg << "\033[0m"; break;
                    case col::G: std::cout << "\033[1;32m" << msg << "\033[0m"; break;
                    case col::Y: std::cout << "\033[1;33m" << msg << "\033[0m"; break;
                    case col::B: std::cout << "\033[1;34m" << msg << "\033[0m"; break;
                    case col::M: std::cout << "\033[1;35m" << msg << "\033[0m"; break;
                    case col::C: std::cout << "\033[1;36m" << msg << "\033[0m"; break;
                    case col::W: std::cout << "\033[0;37m" << msg << "\033[0m"; break;
                    case col::I: std::cout << "\033[1;7m" << msg << "\033[0m"; break;
                }
    #endif
        }

        void progress_bar(unsigned int successes, unsigned int failures)
        {
            unsigned int total = successes + failures;
            unsigned int length = 20;
            float percentage = static_cast<float>(failures) / static_cast<float>(total);
            unsigned int f_bar = static_cast<int>(percentage * static_cast<float>(length));
            if (!f_bar && failures)
                f_bar = 1;
            unsigned int s_bar = length - f_bar;
            output("|");
            if (s_bar)
                output(std::string(s_bar, '='), col::G);
            if (f_bar)
                output(std::string(f_bar, '='), col::R);
            output("| ");
        }

        void header(const test_interface& test)
        {
            output("\n");
            if (!test.group().empty())
                output(test.group() + ".");
            output(test.name() + " ");
        }

        void output_duration(unsigned int duration_ms)
        {
            output("(");
            output(std::to_string(duration_ms));
            output("ms)");
        }

        virtual void case_success(const test_interface& test, const std::string& filename, unsigned int lineNum, const std::string& msg) 
        {
            if (!msg.empty())
            {
                header(test);
                output(filename + ":", col::C);
                output(std::to_string(lineNum), col::Y);
                output(msg + "\n");
            }
        }
        virtual void case_fail(const test_interface& test, const std::string& filename, unsigned int lineNum, const std::string& msg) 
        {
            header(test);
            output(" FAIL. ", col::R);
            output(filename + ":", col::C);
            output(std::to_string(lineNum), col::Y);
            if (!msg.empty())
                output(" " + msg + "\n");
        }
        virtual void case_exception(const test_interface& test, const std::string& exc) 
        {
            header(test);
            output("EXCEPTION", col::M);
            output(" thrown in " + test.exception_location() + "\n");
            output(exc);
        }
        virtual void case_end(const test_interface& test) 
        {
            header(test);
            if (test.exception().empty())
            {
                if (test.failures().empty())
                {
                    output(" PASSED. ", col::G);
                    output(std::to_string(test.successes().size()) + "/" + std::to_string(test.successes().size() + test.failures().size()) + " assertions succeeded.");
                    output_duration(test.duration_ms());
                }
                else
                {
                    output(" FAILED. ", col::R);
                    output(std::to_string(test.failures().size()) + "/" + std::to_string(test.successes().size() + test.failures().size()) + " assertions failed.");
                }
            }
        }
        virtual void testsuite_start(const _::instance& session) 
        {
            unsigned int test_count = 0, group_count = 0;
            for (auto g = session.tests_.begin(); g != session.tests_.end(); ++g, ++group_count)
                test_count += g->second.size();
            if (session.shuffle_)
                output("[SHUFFLE] ", col::C);
            output("Running " + std::to_string(test_count) + " tests (" + std::to_string(group_count) + " groups)\n", col::I);
        }
        virtual void testsuite_end(const _::instance& session) 
        {
            output("\nResults", col::I);
            unsigned int total_successful_assertions = 0, total_failed_assertions = 0, total_test_passes = 0, total_test_failures = 0, total_test_duration_ms = 0;
            for (auto g = session.tests_.begin(); g != session.tests_.end(); ++g)
            {
                unsigned int successful_assertions = 0, failed_assertions = 0, test_passes = 0, test_failures = 0, group_duration_ms = 0;
                for (auto t = g->second.begin(); t != g->second.end(); ++t)
                {
                    successful_assertions += (*t)->successes().size();
                    failed_assertions += (*t)->failures().size();
                    if ((*t)->failures().empty())
                        ++test_passes;
                    else
                        ++test_failures;
                }
                output("\n");
                progress_bar(test_passes, test_failures);
                output(" {" + g->first + "} ");
                output(std::to_string(successful_assertions) + "/" + std::to_string(successful_assertions + failed_assertions) + " assertions passed. ");
                output(std::to_string(test_passes) + "/" + std::to_string(test_passes + test_failures) + " tests passed.");
                output_duration(group_duration_ms);
                total_test_duration_ms += group_duration_ms;
                total_failed_assertions += failed_assertions;
                total_successful_assertions += successful_assertions;
                total_test_passes += test_passes;
                total_test_failures += test_failures;
            }
            output("\nTotal\n", col::I);
            progress_bar(total_test_passes, total_test_failures);
            output(std::to_string(total_successful_assertions) + "/" + std::to_string(total_successful_assertions + total_failed_assertions) + " assertions passed. ");
            output(std::to_string(total_test_passes) + "/" + std::to_string(total_test_passes + total_test_failures) + " tests passed. ");
            output_duration(total_test_duration_ms);
            output("\n");
        }
    };

    //class xUnit : public 

    
    

    

}   // namespace jahoutf

// Macro to initialise jahoutf when using custom main entry func... only needs to be used if not using JAHOUTF_MAIN and instead uses own custom main function.
//  jahoutf_XXX arguments not supported with 
#define JAHOUTF_INIT namespace jahoutf \
{ \
    namespace _ \
    { \
        static std::unique_ptr<instance> session_; \
        void instance::run_tests() { _::run_tests(tests_); } \
    } \
    _::instance& session() \
    { \
        if (!_::session_) \
        { \
            _::session_.reset(new _::instance()); \
            _::session_->event_.reset(new event_interface()); \
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
    for (unsigned int c = 1; c < argc; ++c) \
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
#define RUNALL jahoutf::session().run_tests();
// Shuffle all the tests before invoking them...
#define SHUFFLE jahoutf::session().randomize_ = true; RUNALL
// Override the event observers...
#define SILENT jahoutf::session().event_.reset(new _::instance::event_interface_base());
#define EVENT_INTERFACE(x) jahoutf::session().event_.reset(new x());


// A test case with just name...
#define TESTCLASSNAME(x) x
#define TEST_CASE_DECLARE(testgroup, testname) namespace testgroup \
{ \
    class testname : public jahoutf::test_case \
    {    \
        public: \
        testname() : jahoutf::test_case(#testgroup, #testname) {} \
        void run(); \
    }; \
    testname TESTCLASSNAME(testname); } \
void testgroup::testname::run()

#define TEST_CASE_1(testgroup, testname) TEST_CASE_DECLARE(testgroup, testname)
#define TEST_CASE_2(testname) TEST_CASE_DECLARE(, testname)

#define GET_TEST_CASE_MACRO(_1,_2,TCNAME,...) TCNAME
#define TEST(...) GET_TEST_CASE_MACRO(__VA_ARGS__, TEST_CASE_1, TEST_CASE_2)(__VA_ARGS__)


// Test fixture...
#define TEST_FIXTURE_DECLARE(testfixture, testgroup, testname) namespace testgroup \
{ \
    class testname : public jahoutf::test_interface \
    { \
        public:\
        class wrapper : public testfixture\
        {\
        public:\
            wrapper(jahoutf::test_interface* test) { test_ = test; }\
            void run();\
        };\
        testname() : jahoutf::test_interface(#testgroup, #testname) {}\
        void invoke()\
        {\
            try { wrapper w(this); current_fixture_ = &w; }\
            CATCH(fixture_construct)\
            try { current_fixture_->Setup(); }\
            CATCH(fixture_setup)\
            test_interface::invoke();\
            try { current_fixture_->TearDown(); }\
            CATCH(fixture_teardown)\
        }\
        void run() { current_fixture_->run(); }\
        wrapper* current_fixture_;\
    };\
    testname TESTCLASSNAME(testname); }\
void testgroup::testname::wrapper::run()

#define TEST_FIXTURE_1(testfixture, testgroup, testname) TEST_FIXTURE_DECLARE(testfixture, testgroup, testname)
#define TEST_FIXTURE_2(testfixture, testname) TEST_FIXTURE_DECLARE(testfixture,, testname)

#define GET_TEST_FIXTURE_MACRO(_1,_2,_3, TFNAME,...) TFNAME
#define TEST_F(...) GET_TEST_FIXTURE_MACRO(__VA_ARGS__, TEST_FIXTURE_1, TEST_FIXTURE_2)(__VA_ARGS__)


// Parameterised tests...


// Inline tests...


// Assertions...
#define SUCCESS test_case_report_success(__FILE__, __LINE__, "");
#define FAIL test_case_report_fail(__FILE__, __LINE__, "");
#define EXPECT_EQ(a, b) if (a == b) { SUCCESS } else { FAIL };
#define EXPECT(a) EXPECT_EQ(a, true);
#define EXPECT_NEAR(a, b, e) if (abs(b-a) <= e) { SUCCESS } else { FAIL };

// EXPECT_THROW
// ASSERT_THAT

#endif  // namespace JAHOUTF_HPP