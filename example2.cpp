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


// TEST_A(group, name, jahoutf::values<int>({ 1, 2, 3, 4, 5 }))
// TEST_P(myFixtureWithParam, group, test, jahoutf::values<int>({ 1, 2, 3, 4, 5 }))
//{
//  SUCCESS
//}


// Fixture
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

