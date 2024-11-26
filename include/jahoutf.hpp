/*
MIT License

Copyright (c) 2024 Cordell Barron

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Just Another Header Only Unit Testing Framework (JAHOUTF)
=========================================================

jahoutf is a single header unit testing framework with no dependencies other than C++ STL. There is an in-built test runner which compiles to 
a stand alone executable, as well as xUnit (XML) compatible output to deliver test results. Macros are used extensively, and ultimately I would 
like to make them the same as googletest so, in theory, should be compatible with current googletest tests and can be a drop in replacement.

Please see https://github.com/spiroyster/jahoutf for more information. 

For VisualStudio/MSVC
---------------------
Macro expansion is not correct by default for msvc and requires the standard conforming preprocessor flag.
"/Zc:preprocessor". This can be enabled in VisualStudio projects "Configuration Properties -> C/C++ -> Use Standard Conforming Preprocessor."




*/


#ifndef JAHOUTF_HPP
#define JAHOUTF_HPP

// Stl includes... C++14
#include <algorithm>
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

// Check compiler we are using...
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define JAHOUTF_WIN 1
#include <Windows.h>
#elif defined(__linux__) || defined(__unix__)
#define JAHOUTF_NIX
#endif

//#define JAHOUTF_CONSOLE_NOANSI

// Macro to initialise jahoutf when using custom main entry func... only needs to be used if not using JAHOUTF_MAIN and instead uses own custom main function.
//  jahoutf_XXX arguments not supported with 
#define JAHOUTF_INSTANCE namespace jahoutf \
{ \
    namespace _ { static std::unique_ptr<instance> instance_; } \
	instance& session()  \
	{ \
		if (!_::instance_) \
		{ \
			_::instance_ = std::make_unique<instance>(); \
			_::instance_->event_ = std::make_shared<jahoutf::console>(); \
		}  \
		return *_::instance_; \
	} \
}

// TestRunner. This is the in-built test runner main function which if used, compiles the test suite to a single executable. 
// Custom code can be included in the body before and after RUNALL is called. 
// RUNALL must be done within the macro body in order to run the tests.
#define JAHOUTF_TEST_RUNNER JAHOUTF_INSTANCE \
void test_runner_main(); \
int main(int argc, char** argv) \
{ \
	if (jahoutf::test_runner_arguments(argc, argv, jahoutf::session())) \
		test_runner_main(); \
	return 0; \
} \
void test_runner_main()

#define SILENCE jahoutf::session().silence_ = true;
#define SHUFFLE jahoutf::session().shuffle_ = true;
#define JAHOUTF_REPORT(user_reporter) jahoutf::session().report_.push_back(std::make_shared<user_reporter>());
#define JAHOUTF_EVENT(user_event) jahoutf::session().event_ = std::make_unique<user_event>();
#define RUNALL jahoutf::test_runner_run_tests(jahoutf::session());

#define JAHOUTF_EXCEPTION_CATCHER(location) catch(const std::exception& e) { JAHOUTF_report_exception(__FILE__, __LINE__, "std::exception", #location, e.what()); } \
catch (...) { JAHOUTF_report_exception(__FILE__, __LINE__, "unknown exception", #location, ""); }

// Test Macros (Cases, Fixtures, Parameters and Fixtures using Parameters respectively)...
#define TEST(...) GET_TEST_CASE_MACRO(__VA_ARGS__, TEST_CASE_1, TEST_CASE_2)(__VA_ARGS__)
#define TEST_F(...) GET_TEST_FIXTURE_MACRO(__VA_ARGS__, TEST_FIXTURE_1, TEST_FIXTURE_2)(__VA_ARGS__)
#define TEST_P(...) GET_TEST_VALUES_MACRO(__VA_ARGS__, TEST_VALUES_1, TEST_VALUES_2)(__VA_ARGS__)
#define TEST_FP(...) GET_TEST_F_VALUES_MACRO(__VA_ARGS__, TEST_F_VALUES_1, TEST_F_VALUES_2)(__VA_ARGS__)

// Test body operations...
#define TEST_HALT throw std::runtime_error("Halting test.");
#define REPORT_PASS(assert_type, msg) JAHOUTF_report_pass(__FILE__, __LINE__, assert_type, msg);
#define REPORT_FAIL(assert_type, value, expected, tolerance, msg) JAHOUTF_report_fail(__FILE__, __LINE__, assert_type, value, expected, tolerance, msg);

// Test body assertions (tests can contain any number of assertions)...
#define ASSERT_PASS REPORT_PASS("ASSERT_PASS", "")
#define ASSERT_FAIL REPORT_FAIL("ASSERT_FAIL", "", "", "", "")
#define ASSERT_EQ(x, y) if (x == y) { REPORT_PASS("ASSERT_EQ", "") } else { REPORT_FAIL("ASSERT_EQ", std::to_string(x), std::to_string(y), "", "") };
#define ASSERT_EQ_STR(x, y) if (x.compare(y) == 0) { REPORT_PASS("ASSERT_EQ_STR", "") } else { REPORT_FAIL("ASSERT_EQ_STR", x, y, "", "") };
#define ASSERT_LEQ(x, y) if (x <= y) { REPORT_PASS("ASSERT_LEQ", "") } else { REPORT_FAIL("ASSERT_LEQ", std::to_string(x), std::to_string(y), "", ""); };
#define ASSERT_GEQ(x, y) if (x >= y) { REPORT_PASS("ASSERT_GEQ", "") } else { REPORT_FAIL("ASSERT_GEQ", std::to_string(x), std::to_string(y), "", ""); };
#define ASSERT_LT(x, y) if (x < y) { REPORT_PASS("ASSERT_LT", "") } else { REPORT_FAIL("ASSERT_LT", std::to_string(x), std::to_string(y), "", ""); };
#define ASSERT_GT(x, y) if (x > y) { REPORT_PASS("ASSERT_GT", "") } else { REPORT_FAIL("ASSERT_GT", std::to_string(x), std::to_string(y), "", ""); };
#define ASSERT_NEAR(x, y, e) if (abs(y-x) <= e) { REPORT_PASS("ASSERT_NEAR", "") } else { REPORT_FAIL("ASSERT_NEAR", std::to_string(x), std::to_string(y), std::to_string(e), ""); };
#define ASSERT(x) ASSERT_EQ(x, true);

// googletest macros...
#define SUCCEED REPORT_PASS("SUCCEED", "")
#define FAIL REPORT_FAIL("FAIL", "", "", "", "")
// ADD_FAILURE
// ADD_FAILURE_AT
// EXPECT_THAT
// EXPECT_TRUE
// EXPECT_FALSE
// EXPECT_EQ
// EXPECT_NE
// EXPECT_LE
// EXPECT_GT
// EXPECT_GE
// EXPECT_FLOAT_EQ
// EXPECT_DOUBLE_EQ
// EXPECT_NEAR
// EXPECT_THROW
// EXPECT_ANY_THROW
// EXPECT_NO_THROW
// EXPECT_PRED
// EXPECT_PRED_FORMAT





// Test Case Wrapper Macros... These are wrappers to run the tests. 
#define JAHOUTF_TEST_CASE(name, tags) namespace jahoutf { namespace tests { namespace name \
{ \
	class test_runner_wrapper : public jahoutf::test \
	{ \
	public: \
		test_runner_wrapper() : jahoutf::test(#name, 0, tags) { session().tests_[JAHOUTF_unique_name()] = this; } \
		void JAHOUTF_body() override; \
	}; \
	static test_runner_wrapper trw; \
} } }\
void jahoutf::tests::name::test_runner_wrapper::JAHOUTF_body()

#define TEST_CASE_1(testname,tags) JAHOUTF_TEST_CASE(testname,tags)
#define TEST_CASE_2(testname) JAHOUTF_TEST_CASE(testname,"")
#define GET_TEST_CASE_MACRO(_1,_2,TCNAME,...) TCNAME

// Test values (parameters, array of values)...
#define JAHOUTF_TEST_VALUES(name, params, tags) namespace jahoutf { namespace tests { namespace name \
{ \
	class test_runner_wrapper : public jahoutf::test \
	{ \
	public: \
		test_runner_wrapper() : jahoutf::test("", 0, "") {} \
		test_runner_wrapper(int index) : jahoutf::test(#name, index, tags) { session().tests_[JAHOUTF_unique_name()] = this; } \
		const auto& JAHOUTF_param() { return params.get_values()[JAHOUTF_param_num()]; } \
		unsigned int JAHOUTF_param_total() const { return params.get_total(); } \
		void JAHOUTF_body() override; \
	}; \
	static std::vector<std::shared_ptr<test_runner_wrapper>> instances() \
	{ \
		std::vector<std::shared_ptr<test_runner_wrapper>> result; \
		for (int p = 0; p < params.get_values().size(); ++p) \
			result.push_back(std::make_shared<test_runner_wrapper>(p)); \
		return result; \
	} \
	static std::vector<std::shared_ptr<test_runner_wrapper>> votcw = instances(); \
} } }\
void jahoutf::tests::name::test_runner_wrapper::JAHOUTF_body()

#define TEST_VALUES_1(testname,testvalues,tags) JAHOUTF_TEST_VALUES(testname,testvalues,tags)
#define TEST_VALUES_2(testname,testvalues) JAHOUTF_TEST_VALUES(testname,testvalues,"")
#define GET_TEST_VALUES_MACRO(_1,_2,_3, TVNAME,...) TVNAME

// Test fixture...
#define JAHOUTF_TEST_FIXTURE(name, fixture, tags) namespace jahoutf { namespace tests { namespace name \
{ \
	class test_runner_wrapper : public jahoutf::test, public fixture \
	{ \
	public: \
		test_runner_wrapper() : jahoutf::test(#name, 0, tags) { session().tests_[JAHOUTF_unique_name()] = this; } \
		void JAHOUTF_body() override; \
		void JAHOUTF_run() override \
		{ \
			try { setup(); } \
			JAHOUTF_EXCEPTION_CATCHER(fixture_setup) \
			test::JAHOUTF_run(); \
			try { teardown(); } \
			JAHOUTF_EXCEPTION_CATCHER(fixture_teardown) \
		} \
	}; \
	static test_runner_wrapper trw; \
} } }\
void jahoutf::tests::name::test_runner_wrapper::JAHOUTF_body()

#define TEST_FIXTURE_1(testname,testfixture,tags) JAHOUTF_TEST_FIXTURE(testname,testfixture,tags)
#define TEST_FIXTURE_2(testname,testfixture) JAHOUTF_TEST_FIXTURE(testname,testfixture,"")
#define GET_TEST_FIXTURE_MACRO(_1,_2,_3, TFNAME,...) TFNAME

// Test a single fixture who has access to the params...
#define JAHOUTF_TEST_FIXTURE_VALUES(name,fixture,params,tags) namespace jahoutf { namespace tests { namespace name \
{ \
	class test_runner_wrapper : public jahoutf::test, public fixture \
	{ \
	public: \
		test_runner_wrapper() : jahoutf::test("", 0, "") {} \
		test_runner_wrapper(int index) : jahoutf::test(#name, index, tags) { session().tests_[JAHOUTF_unique_name()] = this; } \
		void JAHOUTF_body() override; \
		const auto& JAHOUTF_param() { return params.get_values()[JAHOUTF_param_num()]; } \
		unsigned int JAHOUTF_param_total() const { return params.get_total(); } \
		void JAHOUTF_run() override \
		{ \
			prep_param(static_cast<const void*>(&JAHOUTF_param())); \
			try { setup(); } \
			JAHOUTF_EXCEPTION_CATCHER(fixture_setup) \
			test::JAHOUTF_run(); \
			try { teardown(); } \
			JAHOUTF_EXCEPTION_CATCHER(fixture_teardown) \
		} \
	}; \
	static std::vector<std::shared_ptr<test_runner_wrapper>> instances() \
	{ \
		std::vector<std::shared_ptr<test_runner_wrapper>> result; \
		for (int p = 0; p < params.get_values().size(); ++p) \
			result.push_back(std::make_shared<test_runner_wrapper>(p)); \
		return result; \
	} \
	static std::vector<std::shared_ptr<test_runner_wrapper>> votcw = instances(); \
} } }\
void jahoutf::tests::name::test_runner_wrapper::JAHOUTF_body()

#define TEST_F_VALUES_1(testname,testfixture,testvalues,tags) JAHOUTF_TEST_FIXTURE_VALUES(testname,testfixture,testvalues,tags)
#define TEST_F_VALUES_2(testname,testfixture,testvalues) JAHOUTF_TEST_FIXTURE_VALUES(testname,testfixture,testvalues,"")
#define GET_TEST_F_VALUES_MACRO(_1,_2,_3,_4, TFVNAME,...) TFVNAME

/// <summary>
/// Jahoutf namespace
/// </summary>
namespace jahoutf
{
	class test;
	class event_interface;
	class report_interface;
	class console;
	class xUnit;

	typedef std::vector<test*> test_list;		

	/// <summary>
	/// Captures the result of a test and contains various information about the test. 
	/// This base result class is assumed to be a pass.
	/// </summary>
	struct result
	{
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="filename">Filename containing test</param>
		/// <param name="line_number">Test line number in file</param>
		/// <param name="t">Test type</param>
		/// <param name="msg">Test message</param>
		result(const std::string& filename, unsigned int line_number, const std::string& t, const std::string& msg)
			: type_(t), msg_(msg), filename_(filename), line_number_(line_number) {
		}

		std::string type_;				/// The test type
		std::string msg_;				/// Test message
		std::string filename_;			/// Test filename
		unsigned int line_number_;		/// Test line number
	};

	typedef result pass;

	/// <summary>
	/// Fail result. Captures the result of a failed test and provides information such as the resultant value,
	/// expected value, and the tolerance used in this test.
	/// </summary>
	struct fail : public result
	{
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="r">The result information (base class)</param>
		/// <param name="value">The resultant value</param>
		/// <param name="expected">The expected value</param>
		/// <param name="tolerance">Tolerance if present</param>
		fail(const result& r, const std::string& value, const std::string& expected, const std::string& tolerance)
			: result(r.filename_, r.line_number_, r.type_, r.msg_), value_(value), expected_(expected), tolerance_(tolerance) {
		}

		std::string value_;				/// The resultant value
		std::string expected_;			/// The expected value
		std::string tolerance_;			/// Tolerance for float comparison
	};

	/// <summary>
	/// Excpetion result is provided if 
	/// </summary>
	struct exception : public result
	{
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="r">The result information (base class)</param>
		/// <param name="location">The location of the thrown exception</param>
		exception(const result& r, const std::string& location)
			: result(r.filename_, r.line_number_, r.type_, r.msg_), location_(location) {
		}

		std::string location_;			/// Location of the exception thrown
	};

	/// <summary>
	/// Summary of tests run.
	/// </summary>
	struct summary
	{
		unsigned int test_passed_ = 0;			/// Total number of tests passed
		unsigned int test_failed_ = 0;			/// Total number of tests failed
		unsigned int test_empty_ = 0;			/// Total number of empty tests (not run)
		unsigned int exceptions_ = 0;			/// Total number of expcetions thrown
		unsigned int assertions_passed_ = 0;	/// Total number of assertions passed
		unsigned int assertions_failed_ = 0;	/// Total number of assertions failed
		unsigned int skipped_ = 0;				/// Total number of skipped tests (not run)
		unsigned int duration_ = 0;				/// Total duration of all tests run
	};

	/// <summary>
	/// Reporting interface. For custom reporting, this interface should be used and can either 
	/// </summary>
	class report_interface
	{
	public:

		/// <summary>
		/// Report, called when the tests have finished.
		/// </summary>
		/// <param name="tests">The tests that were run</param>
		/// <param name="summary">The summary of the tests run</param>
		virtual void report(const test_list& tests, const summary& summary) {}

		/// <summary>
		/// Name of this reporter. Since there can be multiple reporters in a session.
		/// </summary>
		/// <returns></returns>
		virtual std::string name() = 0;
	};

	/// <summary>
	/// Event interface used to hook into events during the testing. A single event interface can be associated with the test runner
	/// and will hook into events that occur during the test runner execution. This can be used to output messages and information about
	/// tests being run. The default event interface used is the inbuilt one which outputs to the console.
	/// </summary>
	class event_interface
	{
	public:

		/// <summary>
		/// Message event. Called at any point provding a generic message.
		/// </summary>
		/// <param name="msg">The message</param>
		virtual void message(const std::string& msg) {}

		/// <summary>
		/// Test case start event. Called when a test starts.
		/// </summary>
		/// <param name="test">The test about to start</param>
		virtual void case_start(const test& test) {}

		/// <summary>
		/// Test success event. Called when a test succeeds/passes.
		/// </summary>
		/// <param name="test">The test that passed</param>
		/// <param name="info">Test pass result</param>
		virtual void assertion_success(const test& test, const pass& info) {}

		/// <summary>
		/// Test fail event. Called when a test fails.
		/// </summary>
		/// <param name="test">The test that failed</param>
		/// <param name="info">Test fail result</param>
		virtual void assertion_fail(const test& test, const fail& info) {}

		/// <summary>
		/// Test exception thrown. Called when a test throws an exception.
		/// </summary>
		/// <param name="test">The test that threw the exception</param>
		/// <param name="info">Test expcetion result</param>
		virtual void case_exception(const test& test, const exception& info) {}

		/// <summary>
		/// Test end event. Called when a test is fnished.
		/// </summary>
		/// <param name="test">The test that has ended</param>
		virtual void case_end(const test& test) {}

		/// <summary>
		/// Test Suite start event. Called when the test session starts.
		/// </summary>
		/// <param name="tests">The session test list</param>
		virtual void suite_start(const test_list& tests) {}

		/// <summary>
		/// Test suite end event. Called when the test session finishes.
		/// </summary>
		/// <param name="tests">The test session list</param>
		/// <param name="summary"></param>
		virtual void suite_end(const test_list& tests, const summary& summary) {}

		/// <summary>
		/// List tests. Called when the "list" argument is passed,
		/// </summary>
		/// <param name="test">The session test list</param>
		/// <param name="list_tags">The tags to list</param>
		virtual void list_test(const test& test, bool list_tags) {}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="tag"></param>
		/// <param name="count"></param>
		virtual void list_tag(const std::string& tag, unsigned int count) {}
	};

	/// <summary>
	/// Session instance. The instance builds a list of tests to run and their associated tags.
	/// It also specifys the event and report interfaces to use.
	/// </summary>
	class instance
	{
	public:
		std::map<std::string, test*> tests_;						// All the tests found in this session (not necessarily run).
		std::shared_ptr<event_interface> event_;					// Event interface subscribing to this session.
		std::vector<std::shared_ptr<report_interface>> report_;		// Report interface(s) subscribing to this session.
		std::vector<std::string> tags_;								// Parsed tags from the arguments.
		std::string test_runner_name_;								// Name of this session test runner.
		std::string test_runner_path_;								// Path of this session test runner.

		// Argument flags...
		bool shuffle_ = false;										// Shuffle flag. True to shuffle tests.
		bool list_ = false;											// List flag. True to list the tests.
		bool list_tags_ = false;									// List tags flag. True to list the session test tags.
		bool silence_ = false;										// Silence flag. True to supress events.

	};

	/// <summary>
	/// Singleton access current instance.
	/// </summary>
	/// <returns>The current instance</returns>
	extern instance& session();

	/// <summary>
	/// Test interface. Base class for all tests.
	/// </summary>
	class test
	{
	public:
		test(const std::string& name, unsigned int index, const std::string& tags) : name_(name), param_index_(index), duration_(0), tags_(" " + tags + " ") {}

		// The test body that is invoked.
		virtual void JAHOUTF_body() {}

		// invoke test...
		virtual void JAHOUTF_run()
		{
			// start timer...
			auto begin = std::chrono::steady_clock::now();

			// Execute the test body...
			try { JAHOUTF_body(); }
			JAHOUTF_EXCEPTION_CATCHER(test_body)
				
			duration_ = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(begin - std::chrono::steady_clock::now()).count());
		}

		// unique name...
		std::string JAHOUTF_unique_name() const 
		{ 
			return name_ + "[" + std::to_string(param_index_) + "]";
		}

		// report pass...
		void JAHOUTF_report_pass(const std::string& filename, unsigned int line_number, const std::string& assert_type, const std::string& msg)
		{
			passes_.push_back(pass(filename, line_number, assert_type, msg));
			jahoutf::session().event_->assertion_success(*this, passes_.back());
		}

		// report fail...
		void JAHOUTF_report_fail(const std::string& filename, unsigned int line_number, const std::string& assert_type, const std::string& value, const std::string& expected, const std::string& tolerance, const std::string& msg)
		{
			fails_.push_back(fail(result(filename, line_number, assert_type, msg), value, expected, tolerance));
			jahoutf::session().event_->assertion_fail(*this, fails_.back());
		}

		// report exception...
		void JAHOUTF_report_exception(const std::string& filename, unsigned int line_number, const std::string& exception_type, const std::string& location, const std::string& what)
		{
			exceptions_.push_back(exception(result(filename, line_number, exception_type, what), location));
			jahoutf::session().event_->case_exception(*this, exceptions_.back());
		}

		// true if the supplied tag is present in the tags...
		bool JAHOUTF_tagged(const std::string& tag) const
		{
			// first check any of our tags...
			bool valid = tags_.find(std::string(" " + tag + " ")) != std::string::npos;
			
			// check the name...
			if (!valid)
				valid = name_ == tag;

			// check if tag is accessing a specfiic param..
			if (!valid)
				valid = JAHOUTF_unique_name() == tag;
				
			return valid;
		}
		const std::string& JAHOUTF_tags() const { return tags_; }

		// run-time test info...
		const std::string& JAHOUTF_name() const { return name_; }
		unsigned int JAHOUTF_param_num() const { return param_index_; }
		virtual unsigned int JAHOUTF_param_total() const { return 1; }

		// results...
		const std::vector<pass>& JAHOUTF_result_passes() const { return passes_; }
		const std::vector<fail>& JAHOUTF_result_fails() const { return fails_; }
		const std::vector<exception>& JAHOUTF_result_exceptions() const { return exceptions_; }
		unsigned int JAHOUTF_result_total() const { return static_cast<unsigned int>(passes_.size() + fails_.size()); }
		unsigned int JAHOUTF_result_duration_ms() const { return duration_; }

	private:
		std::string name_;
		std::string tags_;
		unsigned int param_index_;
		unsigned int duration_;
		std::vector<exception> exceptions_;
		std::vector<pass> passes_;
		std::vector<fail> fails_;
	};

	/// <summary>
	/// The test fixture interface used by a test case. All fixtures must derive from this interface.
	/// </summary>
	class fixture
	{
	public:

		/// <summary>
		/// Setup of this fixture, called before calling the test body to prepare the fixture state and any members for the test.
		/// </summary>
		virtual void setup() {}

		/// <summary>
		/// Teardown of this fixture, called after calling the test body to cleanup/teardown any members/state of this fixture.
		/// </summary>
		virtual void teardown() {}

	protected:

		/// <summary>
		/// Prepare a param 
		/// </summary>
		/// <param name="param"></param>
		virtual void prep_param(const void* param) {}
	};

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class param
	{
		std::vector<T> values_;
	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vs"></param>
		param(const std::vector<T>& vs) : values_(vs) {}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::vector<T>& get_values() const { return values_; }

		unsigned int get_total() const { return static_cast<unsigned int>(values_.size()); }
	};

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class fixture_param : public fixture
	{
		const T* param_value_;
	public:

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const T& JAHOUTF_param()
		{
			if (param_value_)
				return *param_value_;
			throw std::runtime_error("Unable to retrieve param.");
		}

	protected:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="param_value"></param>
		void prep_param(const void* param_value)
		{
			param_value_ = reinterpret_cast<const T*>(param_value);
		}
	};

	/// <summary>
	/// Summerise the tests that were run.
	/// </summary>
	/// <param name="tests">The tests that were run</param>
	/// <param name="all_tests">All the session tests found.</param>
	/// <returns></returns>
	static summary summerise(const test_list& tests, const std::map<std::string, test*>& all_tests)
	{
		summary total;
		for (auto t = 0; t < tests.size(); ++t)
		{
			// Add the assertion totals...
			total.assertions_passed_ += static_cast<unsigned int>(tests[t]->JAHOUTF_result_passes().size());
			total.assertions_failed_ += static_cast<unsigned int>(tests[t]->JAHOUTF_result_fails().size());
			total.exceptions_ += static_cast<unsigned int>(tests[t]->JAHOUTF_result_exceptions().size());
			total.duration_ += tests[t]->JAHOUTF_result_duration_ms();

			if (!tests[t]->JAHOUTF_result_fails().empty() || !tests[t]->JAHOUTF_result_exceptions().empty())
				++total.test_failed_;
			else if (!tests[t]->JAHOUTF_result_passes().empty())
				++total.test_passed_;
			else
				++total.test_empty_;
		}
		total.skipped_ = static_cast<unsigned int>(all_tests.size() - tests.size());
		return total;
	}

	/// <summary>
	/// Prepare and run all the tests to execute as part of the test runner.
	/// </summary>
	/// <param name="session">The session containing the tests to run</param>
	static void test_runner_run_tests(instance& session)
	{
		// If we are silencing, surpress any event interfaces (no notifications)...
		if (session.silence_)
			session.event_.reset(new event_interface());

		// Build a list of tests to run from the all the session tests...
		test_list tests_to_run;
		tests_to_run.reserve(session.tests_.size());

		// If there are tags specificed, use these to build a list of tests to run out of all the session tests.
		if (!session.tags_.empty())
		{
			for (auto itr = session.tests_.begin(); itr != session.tests_.end(); ++itr)
			{
				bool is_tagged = false;
				for (unsigned int t = 0; t < session.tags_.size() && !is_tagged; ++t)
					is_tagged = itr->second->JAHOUTF_tagged(session.tags_[t]);
				if (is_tagged)
					tests_to_run.push_back(itr->second);
			}
		}
		else
		{
			for (auto itr = session.tests_.begin(); itr != session.tests_.end(); ++itr)
				tests_to_run.push_back(itr->second);
		}

		// if we are listing them...
		if (session.list_ || session.list_tags_)
		{
			// If we are listing just the tags...
			if (!session.list_ && session.list_tags_)
			{
				std::string buf;
				std::map<std::string, unsigned int> tags;
				for (unsigned int t = 0; t < tests_to_run.size(); ++t)
				{
					// tokenise the tags...
					std::stringstream ss(tests_to_run[t]->JAHOUTF_tags());
					while (ss >> buf)
						++tags[buf];
				}

				// output the tags...
				for (auto itr = tags.begin(); itr != tags.end(); ++itr)
					session.event_->list_tag(itr->first, itr->second);
			}
			// List the tests in this session...
			else
			{
				for (unsigned int t = 0; t < tests_to_run.size(); ++t)
					session.event_->list_test(*tests_to_run[t], session.list_tags_);
			}
			return;
		}

		// if shuffle, randomise...
		if (session.shuffle_)
			std::random_shuffle(tests_to_run.begin(), tests_to_run.end());

		// run the tests...
		jahoutf::session().event_->suite_start(tests_to_run);
		for (unsigned int t = 0; t < tests_to_run.size(); ++t)
		{
			jahoutf::session().event_->case_start(*tests_to_run[t]);
			tests_to_run[t]->JAHOUTF_run();
			jahoutf::session().event_->case_end(*tests_to_run[t]);
		}
		summary total = summerise(tests_to_run, session.tests_);
		jahoutf::session().event_->suite_end(tests_to_run, total);

		// report
		for (auto reporter = session.report_.begin(); reporter != session.report_.end(); ++reporter)
		{
			try { (*reporter)->report(tests_to_run, total); }
			catch (const std::exception& e) { std::cerr << "Exception thrown in reporter " << (*reporter)->name() << ", " << e.what() << "\n"; }
			catch (...) { std::cerr << "Unknown exception thrown in reporter " << (*reporter)->name() << "\n"; }
		}
	}

	/// <summary>
	/// Process the test runner arguments. Any arguments not processed are assumed to be test tags.
	/// </summary>
	/// <param name="argc">Argument count</param>
	/// <param name="argv">Argument values</param>
	/// <param name="session">The test runner session</param>
	/// <returns>True if testing should proceed</returns>
	static bool test_runner_arguments(int argc, char** argv, instance& session)
	{
		session.test_runner_name_ = std::string(*argv);
#ifdef JAHOUTF_WIN
		auto itr = session.test_runner_name_.rfind('\\');
#endif
#ifdef JAHOUTF_NIX	
		auto itr = session.test_runner_name_.rfind('/');
#endif
		if (itr != 0 && itr != std::string::npos)
		{
			session.test_runner_path_ = session.test_runner_name_.substr(0, itr + 1);
			session.test_runner_name_ = session.test_runner_name_.substr(itr + 1);
		}
		else
			session.test_runner_name_ = "";

		for (int i = 1; i < argc; ++i)
		{
			std::string arg(*(argv + i));
			if (arg == "-silent")
				jahoutf::session().silence_ = true;
			else if (arg == "-tags")
				jahoutf::session().list_tags_ = true;
			else if (arg == "-list")
				jahoutf::session().list_ = true;
			else if (arg == "-shuffle")
				jahoutf::session().shuffle_ = true;
			else if (arg.find("-xunit=") != std::string::npos)
			{
				auto xunit = std::make_shared<xUnit>(arg.substr(7));
				session.report_.emplace_back(xunit);
			}
			else if (arg == "?" || arg == "-help")
			{
				// Output help message...
				std::string str;
				str.append("JAHOUTF test runner.\n");
				str.append("Usage: > " + session.test_runner_name_ + " [-silent] [-list] [-shuffle] [-xunit=\"filename.xml\"] [?] test1 test2 ...\n");
				str.append("\n");
				str.append("-shuffle : shuffle the tests before running them.\n");
				str.append("-silent  : silence events. No console output.\n");
				str.append("-xunit   : specify an xml filename to report xUnit to.\n");
				str.append("-list    : list the tests in this test program.\n");
				str.append("-tags    : list the tags.\n");
				str.append("\n");
				str.append("Using -list and -tags together, will list the tests and associated tags for each test.\n");
				str.append("Pattern matching works on tags, name, and unique name (i.e \"testname[42]\") will match testname with param index 42.\n");
				str.append("All tests except those matching patterns will be disabled/skipped.\n");

				session.event_->message(str);
				return false;
			}
			else
				jahoutf::session().tags_.push_back(arg);
		}
		return true;
	}

	class xUnit : public report_interface
	{
		static void replace_all(std::string& str, const std::string& what, const std::string& with)
		{
			std::size_t itr = str.find(what);
			while (itr != std::string::npos)
			{
				str.replace(itr, itr + what.size(), with.c_str());
				itr = str.find(what);
			}
		}
		static std::string escape_xml_chars(const std::string& syntaxToConvert)
		{
			std::string convertedSyntax = syntaxToConvert;
			replace_all(convertedSyntax, ">", "&gt;");
			replace_all(convertedSyntax, "<", "&lt;");
			replace_all(convertedSyntax, "&", "&amp;");
			replace_all(convertedSyntax, "'", "&apos;");
			replace_all(convertedSyntax, "\"", "&quot;");
			return convertedSyntax;
		}

		class output_testsuite
		{
			std::ostringstream& oss_;
			std::string close_syntax_;
		public:
			output_testsuite(std::ostringstream& oss, const std::string& name, const summary& summary, const std::string& indent)
				: oss_(oss)
			{
				oss_ << "\n" << indent << "<testsuite name=\"" << name << "\" tests=\"" << (summary.test_passed_ + summary.test_failed_ + summary.exceptions_) << "\" failures=\"" << summary.test_passed_ << "\" disabled=\"" << summary.skipped_ << "\" errors=\"" << summary.exceptions_ << "\" time=\"" << summary.duration_ << "\">";
				close_syntax_ = std::string("\n" + indent + "</testsuite>\n");
			}
			~output_testsuite()
			{
				oss_ << close_syntax_;
			}
		};

		class output_testcase
		{
			std::ostringstream& oss_;
			std::string close_syntax_;
		public:
			output_testcase(std::ostringstream& oss, const test& test, const std::string& indent)
				: oss_(oss)
			{
				oss_ << "\n" << indent << "<testcase name=\"" << test.JAHOUTF_unique_name() << "\" status=\"run\" time=\"" << test.JAHOUTF_result_duration_ms() << "\">";
				for (auto f = test.JAHOUTF_result_fails().begin(); f != test.JAHOUTF_result_fails().end(); ++f)
					oss_ << "\n" << indent << "  " << "<failure message=\"" << xUnit::escape_xml_chars(f->msg_) << "\"></failure>";
				oss_ << "\n" << indent << "</testcase>\n";
			}
			~output_testcase()
			{
				oss_ << close_syntax_;
			}
		};

	public:
		std::string filepath_;
		xUnit(const std::string& filename) : filepath_(filename) {}
		std::string name() { return std::string("xUnit xml (" + filepath_ + ")"); }

		void report(const test_list& tests, const summary& summary)
		{
			std::ostringstream oss;
			oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			{
				output_testsuite ts(oss, session().test_runner_name_, summary, "");
				for (unsigned int t = 0; t < tests.size(); ++t)
					output_testcase tc(oss, *tests[t], "  ");
			}
			std::ofstream file(filepath_);
			if (file)
				file << oss.str();
			else
				throw std::runtime_error("Unable to open file " + filepath_);
		}
	};

	class console : public event_interface
	{
#ifdef JAHOUTF_CONSOLE_NOANSI
		std::string red(const std::string& msg) { return msg; }
		std::string green(const std::string& msg) { return msg; }
		std::string yellow(const std::string& msg) { return msg; }
		std::string magenta(const std::string& msg) { return msg; }
		std::string cyan(const std::string& msg) { return msg; }
		std::string blue(const std::string& msg) { return msg; }
#else
		std::string red(const std::string& msg) { return std::string("\033[1;31m" + msg + "\033[0m"); }
		std::string green(const std::string& msg) { return std::string("\033[1;32m" + msg + "\033[0m"); }
		std::string yellow(const std::string& msg) { return std::string("\033[1;33m" + msg + "\033[0m"); }
		std::string magenta(const std::string& msg) { return std::string("\033[1;35m" + msg + "\033[0m"); }
		std::string cyan(const std::string& msg) { return std::string("\033[1;36m" + msg + "\033[0m"); }
		std::string blue(const std::string& msg) { return std::string("\033[1;34m" + msg + "\033[0m"); }
#endif
		std::string header(const test& t) { return t.JAHOUTF_param_total() > 1 ? t.JAHOUTF_unique_name() : t.JAHOUTF_name(); }
		std::string line(const result& r) { return cyan(r.filename_) + ":" + yellow(std::to_string(r.line_number_)); }

	public:
		virtual void message(const std::string& msg) { std::cout << msg; }
		virtual void case_start(const test& test) 
		{
			if (test.JAHOUTF_param_total() > 1 && test.JAHOUTF_param_num() == 0 && !session().shuffle_)
				std::cout << "          " << test.JAHOUTF_name() << "\n";
		}
		virtual void case_end(const test& test)
		{
			if (test.JAHOUTF_result_exceptions().empty() && test.JAHOUTF_result_fails().empty() && !test.JAHOUTF_result_passes().empty())
			{
				std::cout << green("PASSED   ") << " " << header(test) << blue("  (" + std::to_string(test.JAHOUTF_result_duration_ms()) + " ms)");
				if (test.JAHOUTF_result_passes().size() > 1)
					std::cout << "  " << blue(std::to_string(test.JAHOUTF_result_passes().size()) + " assertions.") << "\n";
				else
					std::cout << "\n";
			}
		}
		virtual void assertion_fail(const test& test, const fail& info) 
		{ 
			std::cout << red("FAILED   ") << " " << header(test) << "\n";
			std::cout << line(info) + "\n\t";

			if (info.value_.empty() && info.expected_.empty() && info.tolerance_.empty())
				std::cout << info.type_ + "()\n";
			if (!info.value_.empty() && !info.expected_.empty() && info.tolerance_.empty())
				std::cout << info.type_ + "(" + info.value_ + ", " + info.expected_ + ")\n";
			if (!info.value_.empty() && !info.expected_.empty() && !info.tolerance_.empty())
				std::cout << info.type_ + "(" + info.value_ + ", " + info.expected_ + ") tolerance " + info.tolerance_ + "\n";
			if (!info.msg_.empty())
				std::cout << info.msg_ << "\n";
		}
		virtual void case_exception(const test& test, const exception& info) 
		{ 
			std::cout << magenta("EXCEPTION") << " " << header(test) << "\n";
			std::cout << line(info) + "\n";
			
			if (!info.location_.empty())
				std::cout << " {" << info.location_ << "} ";
			
			if (!info.msg_.empty())
				std::cout << info.msg_;
			std::cout << "\n";
		}
		virtual void suite_start(const test_list& tests) 
		{
			std::cout << "JAHOUTF Running " + std::to_string(tests.size()) + " tests. (type \"?\" for help) ";
			if (session().shuffle_)
				std::cout << cyan(" [Shuffle] ");
			
			unsigned int skipped = static_cast<unsigned int>(session().tests_.size() - tests.size());
			if (skipped)
				std::cout << yellow(" [" + std::to_string(skipped) + " tests are disabled.]");

			std::cout << "\n==============================================================================\n\n";
		}
		virtual void suite_end(const test_list& tests, const summary& summary) 
		{
			std::cout << "\n==============================================================================\n";
			
			// Output results and bar...
			if (summary.test_passed_)
				std::cout << green("    Passed") << " | " << summary.test_passed_ << " (" << summary.assertions_passed_ << "/" << (summary.assertions_passed_ + summary.assertions_failed_) << " Assertions)\n";
			if (summary.test_failed_)
				std::cout << red("    Failed") << " | " << summary.test_failed_ << " (" << summary.assertions_failed_ << "/" << (summary.assertions_passed_ + summary.assertions_failed_) << " Assertions)\n";
			if (summary.exceptions_)
				std::cout << magenta("Exceptions") << " | " << summary.exceptions_ << "\n";
			if (summary.skipped_)
				std::cout << yellow("   Skipped") << " | " << summary.skipped_ << "\n";
			if (summary.test_empty_)
				std::cout << "     Empty" << " | " << summary.test_empty_ << "\n";

			unsigned int totalTests = summary.test_failed_ + summary.test_passed_ + summary.skipped_ + summary.test_empty_;
			std::cout << "     Total | " << summary.test_failed_ + summary.test_passed_ << "/" << totalTests << " tests. " << summary.assertions_passed_ + summary.assertions_failed_ << " assertions. (" << summary.duration_ << " ms).";
			std::cout << "\n==============================================================================\n";
		}
		
		void list_test(const test& test, bool list_tags)
		{
			if (!test.JAHOUTF_param_num())
			{
				std::cout << test.JAHOUTF_name();
				if (!test.JAHOUTF_tags().empty() && list_tags)
					std::cout << "[" << cyan(test.JAHOUTF_tags()) << "]";
				std::cout << "\n";
			}
		}

		void list_tag(const std::string& tag, unsigned int count)
		{
			cyan(tag + "\n");
		}
	};

}

#endif // JAHOUTF_HPP
