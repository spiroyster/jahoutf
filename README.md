# jahoutf
Just another header only unit testing framework. Similar to googletest syntax, supports fixtures, cases, inlined tests.
Output in xUnit xml format..

# Usage

# Quick Example...

#include "jahoutf.hpp"

JAHOUTF_MAIN { RUNALL }

TEST(helloTestWorld, success)
{
    SUCCESS
}
TEST(helloTestWorld, fail)
{
    FAIL
}

Compile it...

> g++ helloTestWorld.cpp -o helloTestWorld.cpp

Run it..

> ./helloTestWorld

Output...




# Test program

## Test Case

The most basic test is a test case. A test case can have 0 or more assertions, all of which must assert true for the test to pass. Test cases must be uniquely named in each group within a test program.

TEST(add)
{
    int a = 1, b = 2;
    EXPECT_EQ(a+b, 3);
}

TEST(subtract)
{
    int a = 1, b = 2;
    EXPECT_EQ(a-b, -1);
}

The test cases can be ordered into groups. This simply allows tests to be grouped and so totals their results.

TEST(arithmetic, add)
{
    int a = 2, b = 1;
    EXPECT_EQ(a+b, 3)
}

TEST(arithmetic, subtract)
{
    int a = 2, b = 1;
    EXPECT_EQ(a-b, 1)
}

## Test Fixture

Fixtures can be used to reduce code. A fixture is a custom class which provides state to a test case and can use custom setup and teardown routines to prep and cleanup for a test case that uses the fixture. To provide a fixture, the fixture class must derive from jahoutf::test_fixture.

class myFixture : public jahoutf::test_fixture
{
    public:
    int x_ = 2, y_ = 1;
};

TEST_F(myFixture, groupB, add) { EXPECT_EQ(x_ + y_, 3); 
TEST_F(myFixture, groupB, subtract) { EXPECT_EQ(x_ - y_, 1); }

Just like test cases, fixture tests can be grouped.

Use the setup and teardown overrides ideally, although constructors and destructors of a fixture class can be used. Fixtuures are
instantiated for each test case so the state is not preserved between fixture tests.

class myFixture : public jahoutf::test_fixture
{
    public:
    void Setup() { x_ = 2; y_ = 1; }
    void TearDown() {}

    int x_, y_;
}

## Parameterised tests



## Command arguments

Command arguments

> ./helloTestWorld -jahoutf_shuffle

> ./helloTestWorld -jahoutf_run="success fail"

> ./helloTestWorld -jahoutf_xUnit=results.xml

> ./helloTestWorld -jahoutf_silent



# Inline tests



# Report


# Event











