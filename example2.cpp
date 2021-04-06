#include "jahoutf.hpp"

// Declare test program main, and run all the tests.
JAHOUTF_TEST_RUNNER { RUNALL }

TEST(oneIsOdd)
{
    int x = 1;
    EXPECT_EQ(x % 2, 1)
}

TEST(twoIsEven)
{
    int x = 2;
    EXPECT_EQ(x % 2, 0)
}

TEST(isOdd, one)
{
    int x = 1;
    EXPECT_EQ(x % 2, 1)
}

TEST(isEven, two)
{
    int x = 2;
    EXPECT_EQ(x % 2, 0)
}


// // Fixtures...
class myFixture : public jahoutf::fixture
{
    public:
    bool IsOdd(int v) { return v % 2 == 1; }
    bool IsEven(int v) { return v % 2 == 0; }
};

TEST_F(isOdd, fixtureOne, myFixture)
{
    int x = 1;
    EXPECT(IsOdd(x))
}

TEST_F(isEven, fixtureTwo, myFixture)
{
    int x = 2;
    EXPECT(IsEven(x))
}


// PAramters

auto oddNumbers = jahoutf::values<int>({1, 3, 5, 7, 9});
auto evenNumbers = jahoutf::values<int>({2, 4, 6, 8, 10});

TEST_VALUES(isOdd, values, oddNumbers)
{
    int x = jahoutf_value();
    EXPECT_EQ(x % 2, 1)
}

TEST_VALUES(isEven, values, evenNumbers)
{
    int x = jahoutf_value();
    EXPECT_EQ(x % 2, 0)
}

TEST_F_VALUES(isOdd, fixtureValues, myFixture, oddNumbers)
{
    int x = jahoutf_value();
    EXPECT(IsOdd(x))
}

TEST_F_VALUES(isEven, fixtureValues, myFixture, evenNumbers)
{
    int x = jahoutf_value();
    EXPECT(IsEven(x))
}


class myFixtureWithParams : public jahoutf::fixture_param<int>
{
    public:
    bool IsOdd(int v) { return v % 2 == 1; }
    bool IsEven(int v) { return v % 2 == 0; }
};

TEST_F_VALUES(isEven, test_f_values_paramFixtureNoParamUse, myFixtureWithParams, evenNumbers)
{
    EXPECT(IsEven(jahoutf_value()))
}

class myFixtureWithParamsAndSetup : public myFixtureWithParams
{
    public:
    void Setup()
    {
        x_ = jahoutf_param();
    }
    int x_;
};

TEST_F_VALUES(isEven, test_f_values_paramFixtureAndSetup, myFixtureWithParamsAndSetup, evenNumbers)
{
    EXPECT(IsEven(x_))
}



