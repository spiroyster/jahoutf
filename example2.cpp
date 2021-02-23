#include "jahoutf.hpp"

// Declare test program main, and run all the tests.
JAHOUTF_MAIN { RUNALL }

// A test that adds x and y together...
TEST(add)
{
    int x = 1, y = 2;
    EXPECT_EQ(x + y, 3)
}

// Test that subtracts y from x...
TEST(subtract)
{
    int x = 1, y = 2;
    EXPECT_EQ(x - y, -1)
}

// Same test as add, however grouped in a category called arithmetic
TEST(arithmetic, add)
{
    int x = 1, y = 2;
    EXPECT_EQ(x + y, 3)
}

TEST(arithmetic, subtract)
{
    int x = 1, y = 2;
    EXPECT_EQ(x + y, 3)
}


TEST_VALUES(test_values, testname, jahoutf::values<int>({ 1, 2, 3, 4, 5 }))
{
    EXPECT_EQ(GetParam(), 5)
}

TEST_VALUES(testname2, jahoutf::values<int>({ 5, 6, 7, 8, 9 }))
{
     EXPECT_EQ(GetParam(), 5)
}


class myFixture : public jahoutf::fixture
{
    public:
    int x_ = 2, y_ = 1;
};

TEST_F(myFixture, fixtureGroup, add)
{
    EXPECT_EQ(x_ + y_, 3);
}

TEST_F(myFixture, fixtureGroup, subtract)
{
    EXPECT_EQ(x_ - y_, 1);
}

TEST_F(myFixture, fixture3)
{
    EXPECT_EQ(y_ - x_, -1);
}

TEST_F_VALUES(testgroup, testname, myFixture, jahoutf::values<int>({ 1, 2, 3, 4, 5 }))
{
    EXPECT_EQ(x_ + GetParam(), 4)
}

TEST_F_VALUES(testname42, myFixture, jahoutf::values<int>({ 2, 2, 0, 2, 2 }))
{
    EXPECT_EQ(x_ + GetParam(), 4)
}




// Fixture, with parameters
class myFixtureWithParam : public jahoutf::param<int>
{
    public:
    void Setup()
    {
        x_ = GetParam();
        y_ = 2;
    }

    protected:
        int x_, y_;
};

// A param test on a fixture with 
TEST_P(myFixtureWithParam, passtest)
{
    SUCCESS
}

TEST_P(myFixtureWithParam, failtest)
{
    FAIL
}

INSTANTIATE_TEST_P(paramtests, test1, passtest, jahoutf::values<int>({ 21, 22, 23, 24 }))
INSTANTIATE_TEST_P(paramtests, test2, failtest, jahoutf::values<int>({ 11, 12, 13, 14 }))
INSTANTIATE_TEST_P(paramtests, test3, passtest, jahoutf::values<int>({ 11, 12, 13, 14 }))


// Parameterized tests.... test body support TEST_A  (array)

// Parameterized tests.... immutable fixture, test body support param    INSTANTIATE_TEST

// Parameterized tests.... mutable fixture, test body and fixture support param     INSTANTIATE_TEST_F

// Parameterized tests.... mutable fixture, param test, test body and fixture support param    TEST_P

// TEST(group, name)             a test case
// TEST_VALUES(group, name, values)           test case for difference values (same test body for multiple items)
// TEST_F(group, name, fixture)           a test fixture (same fixture applied to many different test bodies)
// TEST_F_VALUES(group, name, fixture, values)  test values for same fixture...
// TEST_P(name, fixture)           a param test (on fixture)
// TEST_P_VALUES(group, name, ptest, values)          instantiated test for parameterised values and fixture and test



