#include "../include/LogiQA.hpp"

// Declare test program main, and run all the tests
LOGIQA_TEST_RUNNER { RUNALL }

LOGIQA_TEST(oneIsOdd, "")
{
    int x = 1;
    ASSERT_EQ(x % 2, 1)
}

LOGIQA_TEST(twoIsEven, "")
{
    int x = 2;
	ASSERT_EQ(x % 2, 0)
}

LOGIQA_TEST(one, "isOdd")
{
    int x = 1;
	ASSERT_EQ(x % 2, 1)
}

LOGIQA_TEST(two, "isEven")
{
    int x = 2;
	ASSERT_EQ(x % 2, 0)
}


// // Fixtures...
class myFixture : public logiqa::fixture
{
    public:
    bool IsOdd(int v) { return v % 2 == 1; }
    bool IsEven(int v) { return v % 2 == 0; }
};

LOGIQA_TEST_FIXTURE(fixtureOne, "isOdd", myFixture)
{
    int x = 1;
    ASSERT(IsOdd(x))
}

LOGIQA_TEST_FIXTURE(fixtureTwo, "isEven", myFixture)
{
    int x = 2;
    ASSERT(IsEven(x))
}


// PAramters

auto oddNumbers = logiqa::param<int>({1, 3, 5, 7, 9});
auto evenNumbers = logiqa::param<int>({2, 4, 6, 8, 10});

LOGIQA_TEST_PARAMS(values, "isOdd values", oddNumbers)
{
    int x = logiqa_param();
    ASSERT_EQ(x % 2, 1)
}

LOGIQA_TEST_PARAMS(even_numbers, "isEven values", evenNumbers)
{
    int x = logiqa_param();
    ASSERT_EQ(x % 2, 0)
}

LOGIQA_TEST_FIXTURE_PARAMS(fdjkfjdk, "isOdd fixtureValues", myFixture, oddNumbers)
{
    int x = logiqa_param();
    ASSERT(IsOdd(x))
}

LOGIQA_TEST_FIXTURE_PARAMS(fjdfjdkfdjk, "isEven fixtureValues", myFixture, evenNumbers)
{
    int x = logiqa_param();
    ASSERT(IsEven(x))
}


class myFixtureWithParams : public logiqa::fixture_param<int>
{
    public:
    bool IsOdd(int v) { return v % 2 == 1; }
    bool IsEven(int v) { return v % 2 == 0; }
};

LOGIQA_TEST_FIXTURE_PARAMS(test_f_values_paramFixtureNoParamUse, "isEven", myFixtureWithParams, evenNumbers)
{
    ASSERT(IsEven(logiqa_param()))
}

class myFixtureWithParamsAndSetup : public myFixtureWithParams
{
    public:
    void Setup()
    {
        x_ = logiqa_param();
    }
    int x_;
};

LOGIQA_TEST_FIXTURE_PARAMS(test_f_values_paramFixtureAndSetup, "isEven", myFixtureWithParamsAndSetup, evenNumbers)
{
    ASSERT(IsEven(x_))
}



