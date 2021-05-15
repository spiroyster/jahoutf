#ifndef LOGIQA_HPP
#define LOGIQA_HPP

// Stl includes... C++14
#include <algorithm>
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <chrono>

// Unless specified, by default include default console...
#ifndef LOGIQA_DEFAULT_CONSOLE_EXCLUDE
#define LOGIQA_INCLUDE_DEFAULT_CONSOLE
#endif

// Unless specified, by default include default xUnit...
#ifndef LOGIQA_DEFAULT_XUNIT_EXCLUDE
#define LOGIQA_INCLUDE_DEFAULT_XUNIT
#endif

// Check compiler...
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define LOGIQA_WIN 1
#include <Windows.h>
#elif defined(__linux__) || defined(__unix__)
#define LOGIQA_ANSI 1
#endif

// Macros defining the tests...


#define LOGIQA_TEST(name, tags) namespace logiqa { namespace tests { namespace name \
{ \
	class test_runner_wrapper : public logiqa::test \
	{ \
	public: \
		test_runner_wrapper() : logiqa::test(#name, 0, tags) { logiqa::session().tests_[logiqa_unique_name()] = this; } \
		void logiqa_body() override; \
	}; \
	static test_runner_wrapper trw; \
} } }\
void logiqa::tests::name::test_runner_wrapper::logiqa_body()

// Test array of parameter values...
#define LOGIQA_TEST_PARAMS(name, tags, params) namespace logiqa { namespace tests { namespace name \
{ \
	class test_runner_wrapper : public logiqa::test \
	{ \
	public: \
		test_runner_wrapper() : logiqa::test("", 0, "") {} \
		test_runner_wrapper(int index) : logiqa::test(#name, index, tags) { logiqa::session().tests_[logiqa_unique_name()] = this; } \
		const auto& logiqa_param() { return params.get_values()[logiqa_param_num()]; } \
		void logiqa_body() override; \
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
void logiqa::tests::name::test_runner_wrapper::logiqa_body()

// Test a single fixture
#define LOGIQA_TEST_FIXTURE(name, tags, fixture) namespace logiqa { namespace tests { namespace name \
{ \
	class test_runner_wrapper : public logiqa::test, public fixture \
	{ \
	public: \
		test_runner_wrapper() : logiqa::test(#name, 0, tags) { logiqa::session().tests_[logiqa_unique_name()] = this; } \
		void logiqa_body() override; \
		void logiqa_run() override \
		{ \
			try { setup(); } \
			LOGIQA_EXCEPTION_CATCHER(fixture_setup) \
			test::logiqa_run(); \
			try { teardown(); } \
			LOGIQA_EXCEPTION_CATCHER(fixture_teardown) \
		} \
	}; \
	static test_runner_wrapper trw; \
} } }\
void logiqa::tests::name::test_runner_wrapper::logiqa_body()

// Test a single fixture who has access to the params...
#define LOGIQA_TEST_FIXTURE_PARAMS(name, tags, fixture, params) namespace logiqa { namespace tests { namespace name \
{ \
	class test_runner_wrapper : public logiqa::test, public fixture \
	{ \
	public: \
		test_runner_wrapper() : logiqa::test("", 0, "") {} \
		test_runner_wrapper(int index) : logiqa::test(#name, index, tags) { logiqa::session().tests_[logiqa_unique_name()] = this; } \
		void logiqa_body() override; \
		const auto& logiqa_param() { return params.get_values()[logiqa_param_num()]; } \
		void logiqa_run() override \
		{ \
			prep_param(static_cast<const void*>(&logiqa_param())); \
			try { setup(); } \
			LOGIQA_EXCEPTION_CATCHER(fixture_setup) \
			test::logiqa_run(); \
			try { teardown(); } \
			LOGIQA_EXCEPTION_CATCHER(fixture_teardown) \
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
void logiqa::tests::name::test_runner_wrapper::logiqa_body()


// Test body assertions...
#define TEST_HALT throw std::runtime_error("Halting test.");
#define REPORT_PASS(msg) logiqa_report_pass(__FILE__, __LINE__, msg);
#define REPORT_FAIL(v, e, t, msg) logiqa_report_fail(__FILE__, __LINE__, v, e, t, msg);

#define ASSERT_PASS REPORT_PASS("")
#define ASSERT_FAIL REPORT_FAIL("", "", "", "")
#define ASSERT_EQ(x, y) if (x == y) { ASSERT_PASS } else { REPORT_FAIL(#x, #y, "", ""); };
#define ASSERT_LEQ(x, y) if (x <= y) { ASSERT_PASS } else { REPORT_FAIL(#x, #y, "", ""); };
#define ASSERT_GEQ(x, y) if (x >= y) { ASSERT_PASS } else { REPORT_FAIL(#x, #y, "", ""); };
#define ASSERT_LT(x, y) if (x < y) { ASSERT_PASS } else { REPORT_FAIL(#x, #y, "", ""); };
#define ASSERT_GT(x, y) if (x > y) { ASSERT_PASS } else { REPORT_FAIL(#x, #y, "", ""); };
#define ASSERT_NEAR(x, y, e) if (abs(y-x) <= e) { ASSERT_PASS } else { REPORT_FAIL(#x, #y, #e, ""); };
#define ASSERT(x) ASSERT_EQ(x, true);

//#define EXPECT_EQ(x, y) ASSERT_EQ(x, y) TEST_HALT
//#define EXPECT_LEQ(x, y) ASSERT_LEQ(x, y) TEST_HALT
//#define EXPECT_GEQ(x, y) ASSERT_GEQ(x, y) TEST_HALT
//#define EXPECT_LT(x, y) ASSERT_LT(x, y) TEST_HALT
//#define EXPECT_GT(x, y) ASSERT_GT(x, y) TEST_HALT
//#define EXPECT(x) ASSERT(x) TEST_HALT
//#define EXPECT_NEAR(x, y, e) ASSERT_NEAR(x, y, e) TEST_HALT


// Macro to initialise jahoutf when using custom main entry func... only needs to be used if not using JAHOUTF_MAIN and instead uses own custom main function.
//  jahoutf_XXX arguments not supported with 
#define LOGIQA_INSTANCE namespace logiqa \
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
        } \
        return *_::session_; \
    } \
}

// Macro for defining main function, user can perform custom global setup and teardown (applied to entire test program) also processed -jahoutf arguments.
// Must explicitly call RUNALL, RUNALL_RANDOMIZED or explicit tests?
#define LOGIQA_TEST_RUNNER LOGIQA_INSTANCE void test_runner_main(); \
int main(int argc, char** argv) \
{ \
	logiqa::_::test_runner_arguments(argc, argv); \
    test_runner_main(); \
	return 0; \
} \
void test_runner_main()

// Run all the tests.... used if user has global startup and teardown functionality in main....
#define RUNALL logiqa::_::test_runner_run_tests();

// Run all the tests, silence the output...
#define SILENCE logiqa::session().shuffle_ = true;

// Shuffle the tests...
#define SHUFFLE logiqa::session().shuffle_ = true;

// Add a user reporter...
#define REPORT(user_reporter) logiqa::session().report_.push_back(std::make_shared<user_reporter>());

// Set the user event hooks...
#define EVENT(user_event) logiqa::session().event_.reset(new user_event());

// exception catcher macro...
#define LOGIQA_EXCEPTION_CATCHER(location) catch(const std::exception& e) { logiqa_report_exception(__FILE__, __LINE__, #location, e.what()); } \
catch (...) { logiqa_report_exception(__FILE__, __LINE__, #location, "Unknown exception"); }




// Source code...

// namespace logiqa
namespace logiqa
{

	class test;

	typedef std::vector<test*> test_list;

	namespace results
	{
		struct result
		{
			result(const std::string& filename, unsigned int line_number, const std::string& msg)
				: msg_(msg), filename_(filename), line_number_(line_number) {}

			std::string msg_;
			std::string filename_;
			unsigned int line_number_;
		};

		typedef result pass;

		struct fail : public result
		{
			fail(const result& r, const std::string& value, const std::string& expected, const std::string& tolerance)
				: result(r.filename_, r.line_number_, r.msg_), value_(value), expected_(expected), tolerance_(tolerance) {}

			std::string value_;
			std::string expected_;
			std::string tolerance_;
		};

		struct exception : public result
		{
			exception(const result& r, const std::string& location, const std::string& exception)
				: result(r.filename_, r.line_number_, r.msg_), location_(location), exception_(exception) {}

			std::string location_;
			std::string exception_;
		};

	}

	class report_interface
	{
	public:
		virtual void report() {}
		virtual std::string name() = 0;
	};

	class event_interface
	{
	public:
		virtual void message(const std::string& msg) {}
		virtual void case_start(const test& test) {}
		virtual void case_success(const test& test, const results::pass& info) {}
		virtual void case_fail(const test& test, const results::fail& info) {}
		virtual void case_exception(const test& test, const results::exception& info) {}
		virtual void case_end(const test& test) {}
		virtual void suite_start(const test_list& tests) {}
		virtual void suite_end(const test_list& tests) {}
	};

	namespace _
	{
		class instance
		{
		public:
			std::map<std::string, test*> tests_;
			std::shared_ptr<event_interface> event_;
			std::vector<std::shared_ptr<report_interface>> report_;
			std::vector<std::string> tags_;

			bool shuffle_ = false;
			bool list_ = false;
			bool silence_ = false;

			void report_all()
			{
				for (unsigned int r = 0; r < report_.size(); ++r)
				{
					try { report_[r]->report(); }
					catch (const std::exception& e) { event_->message("Exception thrown in reporter " + report_[r]->name() + ". " + e.what()); }
					catch (...) { event_->message("Exception thrown in reporter " + report_[r]->name()); }
				}
			}

			~instance() { report_all(); }
		};
	}

	extern _::instance& session();

	class test
	{
	public:
		test(const std::string& name, unsigned int index, const std::string& tags) : name_(name), param_index_(index), duration_(0), tags_(" " + tags + " ") {}

		// The test body that is invoked.
		virtual void logiqa_body() {}

		// invoke test...
		virtual void logiqa_run()
		{
			// start timer...
			auto begin = std::chrono::steady_clock::now();
			try { logiqa_body(); }
			LOGIQA_EXCEPTION_CATCHER(test_body)
				auto end = std::chrono::steady_clock::now();
			duration_ = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(begin - end).count());
		}

		// unique name...
		std::string logiqa_unique_name() const { return name_ + "[" + std::to_string(param_index_) + "]"; }

		// report pass...
		void logiqa_report_pass(const std::string& filename, unsigned int line_number, const std::string& msg)
		{
			passes_.push_back(results::pass(filename, line_number, msg));
			logiqa::session().event_->case_success(*this, passes_.back());
		}

		// report fail...
		void logiqa_report_fail(const std::string& filename, unsigned int line_number, const std::string& value, const std::string& expected, const std::string& tolerance, const std::string& msg)
		{
			fails_.push_back(results::fail(results::result(filename, line_number, msg), value, expected, tolerance));
			logiqa::session().event_->case_fail(*this, fails_.back());
		}

		// report exception...
		void logiqa_report_exception(const std::string& filename, unsigned int line_number, const std::string& location, const std::string& msg)
		{
			exceptions_.push_back(results::exception(results::result(filename, line_number, msg), location, ""));
			logiqa::session().event_->case_exception(*this, exceptions_.back());
		}

		// true if the supplied tag is present in the tags...
		bool logiqa_tagged(const std::string& tag) const
		{
			// first check any of our tags...
			bool valid = tags_.find(std::string(" " + tag + " ")) != std::string::npos;
			
			// check the name...
			if (!valid)
				valid = name_ == tag;

			// check if tag is accessing a specfiic param..
			if (!valid)
				valid = logiqa_unique_name() == tag;
				
			return valid;
		}

		// run-time test info...
		const std::string& logiqa_name() const { return name_; }
		unsigned int logiqa_param_num() const { return param_index_; }

		// results...
		const std::vector<results::pass>& logiqa_result_passes() const { return passes_; }
		const std::vector<results::fail>& logiqa_result_fails() const { return fails_; }
		const std::vector<results::exception>& logiqa_result_exceptions() const { return exceptions_; }
		unsigned int logiqa_result_total() const { return static_cast<unsigned int>(passes_.size() + fails_.size()); }
		unsigned int logiqa_result_duration_ms() const { return duration_; }

	private:
		std::string name_;
		std::string tags_;
		unsigned int param_index_;
		unsigned int duration_;
		std::vector<results::exception> exceptions_;
		std::vector<results::pass> passes_;
		std::vector<results::fail> fails_;
	};

	class fixture
	{
	public:
		virtual void setup() {}
		virtual void teardown() {}
	protected:
		virtual void prep_param(const void* param) {}
	};

	template<class T>
	class param
	{
		std::vector<T> values_;
	public:
		param(const std::vector<T>& vs) : values_(vs) {}
		const std::vector<T>& get_values() const { return values_; }
	};

	template<class T>
	class fixture_param : public fixture
	{
		const T* param_value_;
	public:
		const T& logiqa_param()
		{
			if (param_value_)
				return *param_value_;
			throw std::runtime_error("Unable to retrieve param.");
		}
	protected:
		void prep_param(const void* param_value)
		{
			param_value_ = reinterpret_cast<const T*>(param_value);
		}
	};

}






#ifdef LOGIQA_INCLUDE_DEFAULT_CONSOLE
#include <iostream>
#include <sstream>

namespace logiqa
{
	// stdout output...
	class console : public event_interface
	{
	public:

#ifdef LOGIQA_ANSI
		std::string Black(const std::string& msg) { return std::string("\033[1;30m" + msg + "\033[0m"); }
		std::string Red(const std::string& msg) { return std::string("\033[1;31m" + msg + "\033[0m"); }
		std::string Green(const std::string& msg) { return std::string("\033[1;32m" + msg + "\033[0m"); }
		std::string Yellow(const std::string& msg) { return std::string("\033[1;33m" + msg + "\033[0m"); }
		std::string Blue(const std::string& msg) { return std::string("\033[1;34m" + msg + "\033[0m"); }
		std::string Magenta(const std::string& msg) { return std::string("\033[1;35m" + msg + "\033[0m"); }
		std::string Cyan(const std::string& msg) { return std::string("\033[1;36m" + msg + "\033[0m"); }
		std::string White(const std::string& msg) { return std::string("\033[0;37m" + msg + "\033[0m"); }
		std::string Inverse(const std::string& msg) { return std::string("\033[1;7m" + msg + "\033[0m"); }
#elif LOGIQA_WIN
		class colour
		{
		public:
			colour(const std::string& msg, int c)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO cbInfo;
				GetConsoleScreenBufferInfo(hConsole, &cbInfo);
				restore_ = cbInfo.wAttributes;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
				std::cout << msg;
			}
			~colour()
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), restore_);
			}
			int restore_;
		};
		void black(const std::string& msg) { colour(msg, 16); }
		void red(const std::string& msg) { colour(msg, 12); }
		void green(const std::string& msg) { colour(msg, 10); }
		void yellow(const std::string& msg) { colour(msg, 14); }
		void blue(const std::string& msg) { colour(msg, 9); }
		void magenta(const std::string& msg) { colour(msg, 13); }
		void cyan(const std::string& msg) { colour(msg, 11); }
		void white(const std::string& msg) { colour(msg, 15); }
		void inverse(const std::string& msg) { colour(msg, 10); }
#endif

		std::string header(const test& test) { return std::string("\n" + test.logiqa_name() + " "); }
		std::string duration(unsigned int d) { return std::string("(" + std::to_string(d) + "ms)"); }
		virtual void message(const std::string& msg) { std::cout << msg; }
		void results_bar(unsigned int successes, unsigned int failures)
		{
			unsigned int total = successes + failures, length = 20;
			float percentage = static_cast<float>(failures) / static_cast<float>(total);
			unsigned int f_bar = static_cast<int>(percentage * static_cast<float>(length));
			if (!f_bar && failures) { f_bar = 1; }
			unsigned int s_bar = length - f_bar;
			message("|"); green(std::string(s_bar, '=')); red(std::string(f_bar, '=')); message("|");
		}
		virtual void case_success(const test& test, const results::pass& result)
		{
			if (!result.msg_.empty())
				message(header(test)); cyan(result.filename_ + ":"); yellow(std::to_string(result.line_number_)); message(result.msg_ + "\n");
		}
		virtual void case_fail(const test& test, const results::fail& result)
		{
			message(header(test)); red("fail "); cyan(result.filename_ + ":"); yellow(std::to_string(result.line_number_));
			if (!result.value_.empty())     { message("\nValue     | "); white(result.value_); }
			if (!result.expected_.empty())  { message("\nExpected  | "); white(result.expected_); }
			if (!result.tolerance_.empty()) { message("\nTolerance | "); white(result.tolerance_); }
			if (!result.msg_.empty()) { message(result.msg_ + "\n"); }
		}
		virtual void case_exception(const test& test, const results::exception& result)
		{
			message(header(test) + result.exception_ + " thrown in " + result.location_ + " "); cyan(result.filename_ + ":"); yellow(std::to_string(result.line_number_)); message(result.msg_ + "\n");
		}
		virtual void case_end(const test& test)
		{
			message(header(test));
			if (test.logiqa_result_fails().empty())
			{
				green(" PASSED "); message(std::to_string(test.logiqa_result_passes().size()) + "/" + std::to_string(test.logiqa_result_total()) + " assertions succeeded. " + duration(test.logiqa_result_duration_ms()) + '\n');
			}
			else
			{
				red(" FAILED "); message(std::to_string(test.logiqa_result_fails().size()) + "/" + std::to_string(test.logiqa_result_total()) + " assertions failed.\n");
			}
		}
		virtual void suite_start(const test_list& tests_to_run)
		{
			message("Running " + std::to_string(tests_to_run.size()) + " tests. (type \"?\" for help)\n");
			if (session().shuffle_)
				cyan(" [Shuffle] ");
			if (session().shuffle_)
				yellow(" [" + std::to_string(session().tests_.size() - tests_to_run.size()) + " tests are disabled.]\n");
		}

		virtual void suite_end(const test_list& tests_to_run)
		{
			inverse("Total\n");
			unsigned int passed = 0, failed = 0, exceptions = 0, time_taken = 0, tests_passed = 0;
			for (auto t = 0; t < tests_to_run.size(); ++t)
			{
				passed += static_cast<unsigned int>(tests_to_run[t]->logiqa_result_passes().size());
				failed += static_cast<unsigned int>(tests_to_run[t]->logiqa_result_fails().size());
				exceptions += static_cast<unsigned int>(tests_to_run[t]->logiqa_result_exceptions().size());
				time_taken += tests_to_run[t]->logiqa_result_duration_ms();
				if (tests_to_run[t]->logiqa_result_fails().empty() && !tests_to_run[t]->logiqa_result_passes().empty())
					++tests_passed;
			}
			results_bar(passed, failed);
			message(std::to_string(tests_passed) + "/" + std::to_string(tests_to_run.size()) + " tests passed (" + std::to_string(passed) + "/" + std::to_string(passed + failed) + " assertions passed). " + duration(time_taken) + "\n");
			unsigned int skipped = static_cast<unsigned int>(session().tests_.size() - tests_to_run.size());
			if (skipped)
				yellow(std::to_string(skipped) + " tests skipped.\n");
		}
		
	};
}
#endif


// xUnit output...



namespace logiqa
{
	namespace _
	{
		static void test_runner_run_tests()
		{
			// assert there is an event or at least a stub in the event...
			if (session().silence_)
				session().event_.reset(new event_interface());
			else
			{
				// if there is no event speicfied, install the default...
				if (!session().event_)
					session().event_.reset(new console());
			}

			// get all the tests to run...
			test_list tests_to_run;
			tests_to_run.reserve(session().tests_.size());
			if (!session().tags_.empty())
			{
				for (auto itr = session().tests_.begin(); itr != session().tests_.end(); ++itr)
				{
					bool is_tagged = false;
					for (unsigned int t = 0; t < session().tags_.size() && !is_tagged; ++t)
						is_tagged = itr->second->logiqa_tagged(session().tags_[t]);
					if (is_tagged)
						tests_to_run.push_back(itr->second);
				}
			}
			else
			{
				auto inst = session();
				for (auto itr = session().tests_.begin(); itr != session().tests_.end(); ++itr)
					tests_to_run.push_back(itr->second);
			}

			// if we are listing them...
			if (session().list_)
			{
				for (unsigned int t = 0; t < tests_to_run.size(); ++t)
				{
					// only output if not paramter...
					if (!tests_to_run[t]->logiqa_param_num())
						session().event_->message(tests_to_run[t]->logiqa_name() + "\n");
				}
				return;
			}

			// if shuffle, randomise...
			if (session().shuffle_)
				std::random_shuffle(tests_to_run.begin(), tests_to_run.end());
			
			// run the tests...
			logiqa::session().event_->suite_start(tests_to_run);
			for (unsigned int t = 0; t < tests_to_run.size(); ++t)
			{
				logiqa::session().event_->case_start(*tests_to_run[t]);
				tests_to_run[t]->logiqa_run();
				logiqa::session().event_->case_end(*tests_to_run[t]);
			}
			logiqa::session().event_->suite_end(tests_to_run);
		}

		static void test_runner_arguments(int argc, char** argv)
		{
			for (int i = 1; i < argc; ++i)
			{
				std::string arg(*(argv + i));
				if (arg == "-silent")
					logiqa::session().silence_ = true;
				else if (arg == "-list")
					logiqa::session().list_ = true;
				else if (arg == "-shuffle")
					logiqa::session().shuffle_ = true;
				else if (arg == "?" || arg == "-help")
				{
					std::ostringstream oss;
					oss << "LogiQA test runner.\n";
					logiqa::session().event_->message(oss.str());
				}
				else
					logiqa::session().tags_.push_back(arg);
			}
		}
	}
}




#endif // LOGIQA_HPP
