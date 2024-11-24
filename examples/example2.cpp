#include "../include/LogiQA.hpp"

// Test examples showing how to use simple tests, fixtures, parameterised tests (values) 

// Use the in-built test runner to run all the tests...
LOGIQA_TEST_RUNNER { RUNALL }

// Test called "oneIsOdd" which uses an assertion if the value 1 is odd.
LOGIQA_TEST(oneIsOdd, "")
{
    // Use the value of 1...
    int x = 1;

    // Assert if this value is odd. This will be presented as a single test result.
    ASSERT_EQ(x % 2, 1)
}

// Test using an assertion to check if the 2 is even. No tags.
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


// In these examples, we will use a fixture which is an object passed to tests supplying methods which are avalaible
// to all tests using this fixture. The fixture is declared and passed to the test fixture macros. Methods in fixtures 
// must be public in order to accessable to the test fixture. The text macro, wraps the fixture object and so has public
// access to the fixture methods.
class myFixture : public logiqa::fixture
{
public:

    /// <summary>
    /// Check if the value is odd
    /// </summary>
    /// <param name="v">Value to check</param>
    /// <returns>True is value is odd, false otherwise</returns>
    bool IsOdd(int v) { return v % 2 == 1; }

    /// <summary>
    /// Check if the values is even
    /// </summary>
    /// <param name="v">Value to check</param>
    /// <returns>True if values is even, false otherwise</returns>
    bool IsEven(int v) { return v % 2 == 0; }
};

LOGIQA_TEST_FIXTURE(fixtureOne, "isOdd", myFixture)
{
    // Use the value x to 1.
    int x = 1;

    // Use the fixture method IsOdd to check if the value is odd.
    ASSERT(IsOdd(x))
}

LOGIQA_TEST_FIXTURE(fixtureTwo, "isEven", myFixture)
{
    // Set the value x to 2.
    int x = 2;


    ASSERT(IsEven(x))
}


// In these examples we can pass values (parameters) to the test

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
    void setup()
    {
        x_ = logiqa_param();
    }
    int x_;
};

LOGIQA_TEST_FIXTURE_PARAMS(test_f_values_paramFixtureAndSetup, "isEven", myFixtureWithParamsAndSetup, evenNumbers)
{
    ASSERT(IsEven(x_))
}



