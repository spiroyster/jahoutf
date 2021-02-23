# jahoutf
Just another header only unit testing framework. Similar to googletest syntax, supports fixtures, cases, inlined tests.
Output in xUnit xml format..

* Designed to be small
* Single header only, standard c++
* Support xUnit output
* Tests cases, Test fixtures and paramterise these tests.
* Googletest migration support would be nice

# Usage


## Quick Example...

    #include "jahoutf.hpp"

    JAHOUTF_MAIN { RUNALL }     // Declare a jshoutf test program...

    TEST(helloTestWorld, success) { SUCCESS }   // A test that passes...
    TEST(helloTestWorld, fail) { FAIL }         // A test that fails...

Compile it...

> g++ helloTestWorld.cpp -o helloTestWorld.cpp

Run it..

> ./helloTestWorld

Output...





# Creating Tests

Tests can be created for either a test program (whose sole purpose is to perform tests), or tests can be inlined


## Test Case

The most basic test is a test case. A test case can have 0 or more assertions, all of which must assert true for the test to pass. Test cases must be uniquely named in each group within a test program. This can test static and global and functions. This essentially creates a test body for a single test case.

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

The test cases can be ordered into groups. This simply allows tests to be grouped and so totals their results. Tests can be put into namespaces and scoped that way, however for the test runner, a unique identifier (denoted by group and name) is used, ergo if
two tests with the same name are in different namespaces, only a single test will be run (last decalred in the source).

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

## Test parameterised

We can use paramterized tests to save code duplication... Firstly we can pass parameters to a test case

TEST_VALUES(group, name, jahoutf::values<int>({ 1, 2, 3, 4, 5 }))
{
    int x_ = GetParam();
    int y_ = 1;
    EXPECT_EQ(x_, y_)
}

Also we can paramerize fixture tests. These tests use the same fixture and allow the test body to resolve param values during test.

TEST_F_VALUES(group, name, fixture, jahoutf::values<int>({ 1, 2, 3, 4, 5 }))
{
    //GetParam()
}

And lastly, we may want the ability for fixtures to use params as well as just the testbody having access to the params.

class myFixtureWithParams : public jahoutf::param<int>
{

};

TEST_P(myFixtureWithParams, addTest)
{

}

PARAMTERIZE_TEST_P(group, name, addTest, jahoutf::values<int>({ 1, 2, 3, 4, 5 }))






# Running Tests

Command arguments

> ./helloTestWorld -jahoutf_shuffle

> ./helloTestWorld -jahoutf_run="success fail"

> ./helloTestWorld -jahoutf_xUnit=results.xml

> ./helloTestWorld -jahoutf_silent



# Inline/Section Tests

Alternatively, tests can be inlined into existing code

int main(int argc, char** argv)
{
    int a = 2, b = 1;
    int c = a + b;

    TEST_SECTION(group, name)
    {
        EXPECT(c, 3)
    }

    return 0;
}

These will report events and also be recorded and reported hwne the instance closes. Reports can be generated prior by calling...

JAHOUTF_REPORT




# Customising


## Event

## Report











