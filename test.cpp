#include "jahoutf.hpp"

// Introspective testing...
JAHOUTF_TEST_RUNNER { RUNALL }

// Assertion tests
TEST(shouldpass, success) { SUCCESS }
TEST(shouldpass, expect) { EXPECT(true) }
TEST(shouldpass, expect_eq) { EXPECT_EQ(1,1) }

TEST(shouldfail, success) { FAIL }
TEST(shouldfail, expect) { EXPECT(false) }
TEST(shouldfail, expect_eq) { EXPECT_EQ(1,2) }

// Fixture tests
auto nums = jahoutf::values<int>({ 0, 1, 2, 3, 4, 5 });

class foo : public jahoutf::fixture { public: int x_ = 42; };
TEST_F(fixturetest, fixture_test_f_foo, foo) { EXPECT(x_==42) }
TEST_F_VALUES(fixturetest, fixture_test_f_values_foo, foo, nums) { EXPECT_EQ(x_ * jahoutf_value(), 42 * jahoutf_value()) }

class foo2 : public jahoutf::fixture_param<int> { public: int x_ = 42; };
TEST_F(fixturetest, fixture_test_f_foo2, foo2) { EXPECT(x_==42) }
TEST_F_VALUES(fixturetest, fixture_test_f_values_foo2, foo2, nums) { EXPECT_EQ(x_ * jahoutf_value(), 42 * jahoutf_value()) }

class foo3 : public jahoutf::fixture_param<int> 
{ 
    public: 
    void Setup() { x_ = jahoutf_param(); }
    int x_ = 42;
};
TEST_F(fixturetest, fixture_test_f_foo3, foo3) { EXPECT(x_==42) }
TEST_F_VALUES(fixturetest, fixture_test_f_values_foo3, foo3, nums) { EXPECT_EQ(jahoutf_param(), jahoutf_value()) }

// Exception tests
TEST(exceptiontest, testcase_stdexception) { throw std::runtime_error("Std exception throw test"); }

class fixture_construction_exception : public jahoutf::fixture
{
public:
    fixture_construction_exception() { throw std::runtime_error("Fixture construction exception"); }
};

class fixture_setup_exception : public jahoutf::fixture
{
public:
    void Setup() { throw std::runtime_error("Fixture setup exception"); }
};

class fixture_teardown_exception : public jahoutf::fixture
{
public:
    void TearDown() { throw std::runtime_error("Fixture tear down exception"); }
};

TEST_F(exceptiontest, test_fixture_construction_exception, fixture_construction_exception) { FAIL }
TEST_F(exceptiontest, test_fixture_setup_exception, fixture_setup_exception) { FAIL }
TEST_F(exceptiontest, test_fixture_teardown_exception, fixture_teardown_exception) { SUCCESS }


