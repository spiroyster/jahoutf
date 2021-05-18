#include "../include/LogiQA.hpp"

#include <utility>

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
LOGIQA_TEST(assert_fail_fail, "assertion should_fail") { ASSERT_FAIL }

LOGIQA_TEST(assert_pass, "assertion should_pass") 
{ 
	ASSERT(true) 
	ASSERT(1) 
	ASSERT(1.0) 
}

LOGIQA_TEST(assert_fail, "assertion should_fail") 
{ 
	ASSERT(false) 
	ASSERT(0)
	ASSERT(0.0)
}

LOGIQA_TEST(assert_eq_pass, "assertion assert_eq should_pass") 
{ 
	ASSERT_EQ(1, 1) 
	ASSERT_EQ(1.0, 1) 
	ASSERT_EQ(true, true) 
	ASSERT_EQ(1, true) 
	ASSERT_EQ(true, 1) 
	ASSERT_EQ(false, false) 
}

LOGIQA_TEST(assert_eq_fail, "assertion assert_eq should_fail") 
{ 
	ASSERT_EQ(1, 0) 
	ASSERT_EQ(1.0, 0) 
	ASSERT_EQ(true, false) 
	ASSERT_EQ(1, false) 
	ASSERT_EQ(true, 0) 
	ASSERT_EQ(false, true) 
}


LOGIQA_TEST(assert_leq_pass, "assertion assert_leq should_pass")
{
	// eq...
	ASSERT_LEQ(0, 0)
	ASSERT_LEQ(0, 0.0)
	ASSERT_LEQ(0.0, 0)
	ASSERT_LEQ(-1, -1.0)
	ASSERT_LEQ(-1, -1)
	ASSERT_LEQ(-1.0, -1)
	ASSERT_LEQ(-1.0, -1.0)
	ASSERT_LEQ(2.0, 2)
	ASSERT_LEQ(2, 2.0)
	ASSERT_LEQ(2, 2)
	ASSERT_LEQ(2.0, 2.0)

	// < -1
	ASSERT_LEQ(-2, -1)
	ASSERT_LEQ(-2.0, -1)
	ASSERT_LEQ(-2.0, -1.0)
	ASSERT_LEQ(-2, -1.0)

	// < 0
	ASSERT_LEQ(-1, 0)
	ASSERT_LEQ(-1.0, 0)
	ASSERT_LEQ(-1.0, 0.0)
	
	// < 1
	ASSERT_LEQ(0, 1)
	ASSERT_LEQ(0.0, 1)
	ASSERT_LEQ(0.0, 1.0)
	ASSERT_LEQ(0, 1.0)

	// < 2
	ASSERT_LEQ(1, 2)
	ASSERT_LEQ(1.0, 2)
	ASSERT_LEQ(1.0, 2.0)
	ASSERT_LEQ(1, 2.0)
}

LOGIQA_TEST(assert_leq_fail, "assertion assert_leq should_fail")
{
	// < -1
	ASSERT_LEQ(-1, -2)
	ASSERT_LEQ(-1, -2.0)
	ASSERT_LEQ(-1.0, -2.0)
	ASSERT_LEQ(-1.0, -2)

	// < 0
	ASSERT_LEQ(0, -1)
	ASSERT_LEQ(0, -1.0)
	ASSERT_LEQ(0.0, -1.0)

	// < -1
	ASSERT_LEQ(-1, -2)
	ASSERT_LEQ(-1, -2.0)
	ASSERT_LEQ(-1.0, -2.0)
	ASSERT_LEQ(-1.0, -2)
	
	// < 2
	ASSERT_LEQ(2, 1)
	ASSERT_LEQ(2, 1.0)
	ASSERT_LEQ(2.0, 1.0)
	ASSERT_LEQ(2.0, 1)
}

LOGIQA_TEST(assert_geq_pass, "assertion assert_geq should_pass")
{
	// eq...
	ASSERT_GEQ(0, 0)
	ASSERT_GEQ(0, 0.0)
	ASSERT_GEQ(0.0, 0)
	ASSERT_GEQ(-1, -1.0)
	ASSERT_GEQ(-1, -1)
	ASSERT_GEQ(-1.0, -1)
	ASSERT_GEQ(-1.0, -1.0)
	ASSERT_GEQ(2.0, 2)
	ASSERT_GEQ(2, 2.0)
	ASSERT_GEQ(2, 2)
	ASSERT_GEQ(2.0, 2.0)

	// < -1
	ASSERT_GEQ(-1, -2)
	ASSERT_GEQ(-1, -2.0)
	ASSERT_GEQ(-1.0, -2.0)
	ASSERT_GEQ(-1.0, -2)

	// < 0
	ASSERT_GEQ(0, -1)
	ASSERT_GEQ(0, -1.0)
	ASSERT_GEQ(0.0, -1.0)

	// < -1
	ASSERT_GEQ(-1, -2)
	ASSERT_GEQ(-1, -2.0)
	ASSERT_GEQ(-1.0, -2.0)
	ASSERT_GEQ(-1.0, -2)
	
	// < 2
	ASSERT_GEQ(2, 1)
	ASSERT_GEQ(2, 1.0)
	ASSERT_GEQ(2.0, 1.0)
	ASSERT_GEQ(2.0, 1)
}

LOGIQA_TEST(assert_geq_fail, "assertion assert_geq should_fail")
{
	// < -1
	ASSERT_GEQ(-2, -1)
	ASSERT_GEQ(-2.0, -1)
	ASSERT_GEQ(-2.0, -1.0)
	ASSERT_GEQ(-2, -1.0)

	// < 0
	ASSERT_GEQ(-1, 0)
	ASSERT_GEQ(-1.0, 0)
	ASSERT_GEQ(-1.0, 0.0)
	
	// < 1
	ASSERT_GEQ(0, 1)
	ASSERT_GEQ(0.0, 1)
	ASSERT_GEQ(0.0, 1.0)
	ASSERT_GEQ(0, 1.0)

	// < 2
	ASSERT_GEQ(1, 2)
	ASSERT_GEQ(1.0, 2)
	ASSERT_GEQ(1.0, 2.0)
	ASSERT_GEQ(1, 2.0)
}


LOGIQA_TEST(assert_gt_pass, "assertion assert_gt should_pass")
{
	// < -1
	ASSERT_GT(-1, -2)
	ASSERT_GT(-1, -2.0)
	ASSERT_GT(-1.0, -2.0)
	ASSERT_GT(-1.0, -2)

	// < 0
	ASSERT_GT(0, -1)
	ASSERT_GT(0, -1.0)
	ASSERT_GT(0.0, -1.0)

	// < -1
	ASSERT_GT(-1, -2)
	ASSERT_GT(-1, -2.0)
	ASSERT_GT(-1.0, -2.0)
	ASSERT_GT(-1.0, -2)
	
	// < 2
	ASSERT_GT(2, 1)
	ASSERT_GT(2, 1.0)
	ASSERT_GT(2.0, 1.0)
	ASSERT_GT(2.0, 1)
}

LOGIQA_TEST(assert_gt_fail, "assertion assert_gt should_fail")
{
	// eq...
	ASSERT_GT(0, 0)
	ASSERT_GT(0, 0.0)
	ASSERT_GT(0.0, 0)
	ASSERT_GT(-1, -1.0)
	ASSERT_GT(-1, -1)
	ASSERT_GT(-1.0, -1)
	ASSERT_GT(-1.0, -1.0)
	ASSERT_GT(2.0, 2)
	ASSERT_GT(2, 2.0)
	ASSERT_GT(2, 2)
	ASSERT_GT(2.0, 2.0)

	// < -1
	ASSERT_GT(-2, -1)
	ASSERT_GT(-2.0, -1)
	ASSERT_GT(-2.0, -1.0)
	ASSERT_GT(-2, -1.0)

	// < 0
	ASSERT_GT(-1, 0)
	ASSERT_GT(-1.0, 0)
	ASSERT_GT(-1.0, 0.0)
	
	// < 1
	ASSERT_GT(0, 1)
	ASSERT_GT(0.0, 1)
	ASSERT_GT(0.0, 1.0)
	ASSERT_GT(0, 1.0)

	// < 2
	ASSERT_GT(1, 2)
	ASSERT_GT(1.0, 2)
	ASSERT_GT(1.0, 2.0)
	ASSERT_GT(1, 2.0)
}

LOGIQA_TEST(assert_lt_pass, "assertion assert_lt should_pass")
{
	// < -1
	ASSERT_LT(-2, -1)
	ASSERT_LT(-2.0, -1)
	ASSERT_LT(-2.0, -1.0)
	ASSERT_LT(-2, -1.0)

	// < 0
	ASSERT_LT(-1, 0)
	ASSERT_LT(-1.0, 0)
	ASSERT_LT(-1.0, 0.0)
	
	// < 1
	ASSERT_LT(0, 1)
	ASSERT_LT(0.0, 1)
	ASSERT_LT(0.0, 1.0)
	ASSERT_LT(0, 1.0)

	// < 2
	ASSERT_LT(1, 2)
	ASSERT_LT(1.0, 2)
	ASSERT_LT(1.0, 2.0)
	ASSERT_LT(1, 2.0)
}

LOGIQA_TEST(assert_lt_fail, "assertion assert_lt should_fail")
{
	// eq...
	ASSERT_LT(0, 0)
	ASSERT_LT(0, 0.0)
	ASSERT_LT(0.0, 0)
	ASSERT_LT(-1, -1.0)
	ASSERT_LT(-1, -1)
	ASSERT_LT(-1.0, -1)
	ASSERT_LT(-1.0, -1.0)
	ASSERT_LT(2.0, 2)
	ASSERT_LT(2, 2.0)
	ASSERT_LT(2, 2)
	ASSERT_LT(2.0, 2.0)
	
	// < -1
	ASSERT_LT(-1, -2)
	ASSERT_LT(-1, -2.0)
	ASSERT_LT(-1.0, -2.0)
	ASSERT_LT(-1.0, -2)

	// < 0
	ASSERT_LT(0, -1)
	ASSERT_LT(0, -1.0)
	ASSERT_LT(0.0, -1.0)

	// < -1
	ASSERT_LT(-1, -2)
	ASSERT_LT(-1, -2.0)
	ASSERT_LT(-1.0, -2.0)
	ASSERT_LT(-1.0, -2)
	
	// < 2
	ASSERT_LT(2, 1)
	ASSERT_LT(2, 1.0)
	ASSERT_LT(2.0, 1.0)
	ASSERT_LT(2.0, 1)
}

struct assert_near_test_data
{
	assert_near_test_data(double value, double expected, double tolerance) : v_(value), e_(expected), t_(tolerance) {}
	float v_, e_, t_;
};

// Near
LOGIQA_TEST_PARAMS(assert_near_pass, "assertion assert_near should_pass", logiqa::param<assert_near_test_data>({
	assert_near_test_data(0.1, 0.1, 0.0),
	assert_near_test_data(0.1, 0.1, 0.0),
	assert_near_test_data(0.1, 0.1, 0.0),
	assert_near_test_data(0.1, 0.1, 0.0)
}))
{
	const auto& p = logiqa_param();
	ASSERT_NEAR(p.v_, p.e_, p.t_);
}

LOGIQA_TEST_PARAMS(assert_near_fail, "assertion assert_near should_fail", logiqa::param<assert_near_test_data>({
	assert_near_test_data(0.1, 0.1, 1.0),
	assert_near_test_data(0.1, 0.1, 1.0),
	assert_near_test_data(0.1, 0.1, 1.0),
	assert_near_test_data(0.1, 0.1, 1.0)
}))
{
	const auto& p = logiqa_param();
	ASSERT_NEAR(p.v_, p.e_, p.t_);
}


// Halt
LOGIQA_TEST(halt, "halt should_throw")
{
	TEST_HALT
}

LOGIQA_TEST(halt_at_start, "halt should_throw")
{
	TEST_HALT
	ASSERT_FAIL
}

LOGIQA_TEST(halt_at_end, "halt should_throw")
{
	ASSERT_PASS
	TEST_HALT
	ASSERT_FAIL
}

