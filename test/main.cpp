#include "../include/LogiQA.hpp"

// Introspective testing...
LOGIQA_TEST_RUNNER { RUNALL }




// assertion tests...

// expect tests...

// halt tests




// param tests...


// fixture tests...
// fixture and params tests...

// exception tests










// // Assertion tests
// LOGIQA_TEST(pass, "shouldpass") { ASSERT_PASS }
// LOGIQA_TEST(assert_true, "shouldpass") { ASSERT(true) }
// LOGIQA_TEST(assert_eq_1_1, "shouldpass") { ASSERT_EQ(1,1) }

// LOGIQA_TEST(fail, "shouldfail") { ASSERT_FAIL }
// LOGIQA_TEST(assert_false, "shouldfail") { ASSERT(false) }
// LOGIQA_TEST(assert_eq_1_2, "shouldfail") { ASSERT_EQ(1,2) }

// // Fixture tests
// auto nums = logiqa::param<int>({ 0, 1, 2, 3, 4, 5 });

// class foo : public logiqa::fixture { public: int x_ = 42; };
// LOGIQA_TEST_FIXTURE(fixture_test_f_foo, "fixturetest", foo) { ASSERT(x_==42) }
// LOGIQA_TEST_FIXTURE_PARAMS(fixture_test_f_values_foo, "fixturetest", foo, nums) { ASSERT_EQ(x_ * logiqa_param(), 42 * logiqa_param()) }


// class foo2 : public logiqa::fixture_param<int> { public: int x_ = 42; };
// LOGIQA_TEST_FIXTURE(fixture_test_f_foo2, "fixturetest", foo2) { ASSERT(x_==42) }
// LOGIQA_TEST_FIXTURE_PARAMS(fixture_test_f_values_foo2, "fixturetest", foo2, nums) { ASSERT_EQ(x_ * logiqa_param(), 42 * logiqa_param()) }

// class foo3 : public logiqa::fixture_param<int> 
// { 
//    public: 
//    void setup() { x_ = logiqa_param(); }
//    int x_ = 42;
// };
// LOGIQA_TEST_FIXTURE(fixture_test_f_foo3, "fixturetest", foo3) { ASSERT(x_==42) }
// LOGIQA_TEST_FIXTURE_PARAMS(fixture_test_f_values_foo3, "fixturetest", foo3, nums) { ASSERT_EQ(logiqa_param(), logiqa_param()) }

// // Exception tests
// LOGIQA_TEST(testcase_stdexception, "exceptiontest") { throw std::runtime_error("Std exception throw test"); }

// class fixture_setup_exception : public logiqa::fixture
// {
// public:
//    void setup() { throw std::runtime_error("Fixture setup exception"); }
// };

// class fixture_teardown_exception : public logiqa::fixture
// {
// public:
//    void tearDown() { throw std::runtime_error("Fixture tear down exception"); }
// };

// LOGIQA_TEST_FIXTURE(test_fixture_setup_exception, "exceptiontest", fixture_setup_exception) { ASSERT_FAIL }
// LOGIQA_TEST_FIXTURE(test_fixture_teardown_exception, "exceptiontest", fixture_teardown_exception) { ASSERT_PASS }


