#include "../include/LogiQA.hpp"

//LOGIQA_TEST_RUNNER{ RUNALL }

//#define TEST_HALT throw std::runtime_error("Halting test.");
//#define REPORT_PASS(assert_type, msg) logiqa_report_pass(__FILE__, __LINE__, assert_type, msg);
//#define REPORT_FAIL(assert_type, value, expected, tolerance, msg) logiqa_report_fail(__FILE__, __LINE__, assert_type, value, expected, tolerance, msg);
//
//#define ASSERT_PASS REPORT_PASS("ASSERT_PASS", "")
//#define ASSERT_FAIL REPORT_FAIL("ASSERT_FAIL", "", "", "", "")
//#define ASSERT_EQ(x, y) if (x == y) { REPORT_PASS("ASSERT_EQ", "") } else { REPORT_FAIL("ASSERT_EQ", std::to_string(x), std::to_string(y), "", "") };
//#define ASSERT_LEQ(x, y) if (x <= y) { REPORT_PASS("ASSERT_LEQ", "") } else { REPORT_FAIL("ASSERT_LEQ", std::to_string(x), std::to_string(y), "", ""); };
//#define ASSERT_GEQ(x, y) if (x >= y) { REPORT_PASS("ASSERT_GEQ", "") } else { REPORT_FAIL("ASSERT_GEQ", std::to_string(x), std::to_string(y), "", ""); };
//#define ASSERT_LT(x, y) if (x < y) { REPORT_PASS("ASSERT_LT", "") } else { REPORT_FAIL("ASSERT_LT", std::to_string(x), std::to_string(y), "", ""); };
//#define ASSERT_GT(x, y) if (x > y) { REPORT_PASS("ASSERT_GT", "") } else { REPORT_FAIL("ASSERT_GT", std::to_string(x), std::to_string(y), "", ""); };
//#define ASSERT_NEAR(x, y, e) if (abs(y-x) <= e) { REPORT_PASS("ASSERT_NEAR", "") } else { REPORT_FAIL("ASSERT_NEAR", std::to_string(x), std::to_string(y), std::to_string(e), ""); };
//#define ASSERT(x) ASSERT_EQ(x, true);


LOGIQA_TEST(assert_pass_pass, "assertion should_pass") { ASSERT_PASS }
LOGIQA_TEST(assert_pass, "assertion should_pass") { ASSERT(true) ASSERT(1) ASSERT(1.0) ASSERT(-1.0) ASSERT(-1) }
LOGIQA_TEST(assert_eq_pass, "assertion should_pass") { ASSERT_EQ(1, 1) ASSERT_EQ(1.0, 1) ASSERT_EQ(true, true) ASSERT_EQ(1, true) ASSERT_EQ(true, 1) ASSERT_EQ(false, false) }
LOGIQA_TEST(assert_leq_pass, "assertion should_pass") { ASSERT_LEQ(-1, 0) ASSERT_LEQ(0, 0) ASSERT_LEQ(-0.5, 0.0) ASSERT_LEQ(-1, 0.0) }
LOGIQA_TEST(assert_geq_pass, "assertion should_pass") { ASSERT_GEQ(1, 0) ASSERT_GEQ(0, 0) ASSERT_GEQ(0.5, 0.0) ASSERT_GEQ(1, 0.0) }
LOGIQA_TEST(assert_lt_pass, "assertion should_pass") { ASSERT_LT(-1, 0) ASSERT_LT(-1.0, 0) }
LOGIQA_TEST(assert_gt_pass, "assertion should_pass") { ASSERT_GT(1, 0) ASSERT_GT(1.0, 0) }








namespace logiqa 
{ 
	namespace tests 
	{ 
		namespace name
		{ 
			class test_runner_wrapper : public logiqa::test
			{
			public:
				test_runner_wrapper() : logiqa::test("myname", 0, "") 
				{ 
					session().tests_[logiqa_unique_name()] = this; 
					auto inst = session();

					int y = 0;
					++y;
				}
				void logiqa_body() override;
			};
			static test_runner_wrapper trw;

		} 
	} 
}
void logiqa::tests::name::test_runner_wrapper::logiqa_body()
{
	int y = 0;
	++y;
	
	ASSERT(true);
}







