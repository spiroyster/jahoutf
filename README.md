# jahoutf
Just another header only unit testing framework. xUnit style single header only test framework featuring:

* Lightweight
* Header only
* C++11 and STL only
* xUnit output
* Test runner
* Supports fixtures, paramterised and inline/section tests.
* Customisable output and reporting.
* Test Shuffling

## Usage

It is a single header only lib requiring C++14 or later, so simply include jahoutf.hpp

Quick example:

    // Only need to include jahoutf.hpp
    #include "jahoutf.hpp"

    // Initialise the test runner and run all the tests...
    JAHOUTF_TEST_RUNNER { RUNALL }

    // A single test with a single assertion that passes...
    TEST(helloWorld) { SUCCESS }

Compile it...

    > g++ helloTestWorld.cpp -o helloTestWorld.cpp

Run it....

    > ./helloTestWorld

Run it again, this time report xUnit xml...

    > ./helloTestWorld -xml=results.xml



# Creating Tests

Tests can either be written in a test program (and thus run by the test runner), or they can be written into sections within existing code to run tests in-situ. The syntax for declaring a test and implmenting test body is as follows:

    TEST_x(group, name, ...)
    {
        // Test body
    }

*TEST_x* is a macro that can be **TEST**, **TEST_F**, **TEST_P**, **SECTION** or their paramterised equivalents **TEST_VALUES**, **TEST_F_VALUES** or **TEST_P_VALUES**. SECTION tests follow the same syntax format as above (note no paramterised tests for *SECTION*). The test body should contain one or more assertions and can contain any other logic required.

The test runner identifies individual tests by a combination of group name and test name. Not assigning a group is valid, however every test must have a name. For this reason test uniqueness is defined solely by the group and name values supplied to the macro, and while test code/objects can scoped via namespaces, the test runner will ignore only uniquely identify via group and name. Valid names must follow C++ syntax rules for typedef naming.

To create a test program, simply delcare a single test runner main function.

    JAHOUTF_TEST_RUNNER { RUNALL }

The main block can contain any user code required but must declare either RUNALL (or SHUFFLE) in order to actually invoke tests. This is essentially the main entry for the program and only this is required (no other main function should be used or implemented).

# Test Case

The most basic test is a test case. A test case can have 0 or more assertions, all of which must assert true for the test to pass.

    TEST(isOdd)
    {
        int x = 1;
        EXPECT_EQ(x % 2, 1)
    }

The test cases can be grouped. This 

This simply allows tests to be grouped and so totals their results. Tests can be put into namespaces and scoped that way, however for the test runner, a unique identifier (denoted by group and name) is used, ergo if
two tests with the same name are in different namespaces, only a single test will be run (last decalred in the source).

    TEST(isOdd, one)
    {
        int x = 1;
        EXPECT_EQ(x % 2, 1)
    }

    TEST(isOdd, two)
    {
        int x = 2;
        EXPECT_EQ(x % 2, 1)
    }

There is no other way currently supported (although tags might be nice)

# Test Fixture

* Constructed/Destruted each test case
* Init/Free code ideally should go in the Setup/TearDown not contructor/destructor
* Can contain members, methods
* Test body has 'inherited' access to fixture.
* They are mutable from perspective of the test body


A fixture is a custom class which provides state to a test case and can use custom setup and teardown routines to prep and cleanup for a test case that uses the fixture. To provide a fixture, the fixture class must derive from jahoutf::test_fixture

    class foo : public jahoutf::test_fixture
    {
        public:
        bool IsOdd(int v) { return v % 2; }
        bool IsEven(int v) { return !IsOdd(v); }
    };

    TEST_F(isOdd, one, foo) { EXPECT(IsOdd(1)); 
    TEST_F(isOdd, two, foo) { EXPECT(IsOdd(2)); 

    TEST_F(isEven, one, foo) { EXPECT(IsEven(1)); 
    TEST_F(isEven, two, foo) { EXPECT(IsEven(2)); 


Fixtures are constructed and destructed each test case. This can be done within a safe environment for testing in case issue with setup/teardown etc... (remember, destructors must NOT throw)

Fixtures are mutable, although since longevity is clearly defined they will be destructed anyway before next test.

Fixtures can contain members, methods, other objects etc. The test body inherits access to the fixture object, so fxiture members/methods can be private, however there must be some publicly accessable resources to give




# Parameterised Tests

Parameters need to be wrapped in a jahoutf::values<> object which define the container for passing th different values to the test.

    auto oddNums = jahoutf::values<int>({ 1, 3, 5, 7, 9 });





* Values are immutable
* Each passed once to the test body.
* Fixtures must derive from jahoutf::fixture_param<> to access the value (not avaliabel in constructor/destructor only setup/teardown)
* Fixtures can only access the param through jahoutf_param();
* Test body can use either jahoutf_param() or jahoutf_value()
* Fixtures must use same type as values type.


# Running Tests

Command arguments

> ./helloTestWorld -shuffle

> ./helloTestWorld -silent

> ./helloTestWorld -xUnit=results.xml

> ./helloTestWorld -list

> ./helloTestWorld shouldpass.*



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











