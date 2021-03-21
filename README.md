# jahoutf
Just another header only unit testing framework. xUnit style single header only test framework featuring:

* Lightweight
* Header only
* STL only
* xUnit output
* Test runner
* Fixtures supported
* Parameterised values suported.
* Customisable output and reporting.
* Test Shuffling

## Example
Write a test program with a single test...

    // Only need to include jahoutf.hpp
    #include "jahoutf.hpp"

    // Initialise the test runner and run all the tests...
    JAHOUTF_TEST_RUNNER { RUNALL }

    // A single test with a single assertion that passes...
    TEST(helloWorld) { SUCCESS }

Compile it...

    > g++ helloTestWorld.cpp -o helloTestWorld

Run it....

    > ./helloTestWorld

Run it again, this time report xUnit xml...

    > ./helloTestWorld -xml=results.xml

Resultant xUnit xml...





# Creating Tests

Tests can either be written in a test program (and thus run by the test runner), or they can be written into sections within existing code to run tests in-situ. The syntax for declaring a test and implmenting a test is as follows:

    TEST_x(group, name, ...)
    {
        // Test body
    }

*TEST_x* is a macro that can be **TEST** for test cases, **TEST_F** for test fixtures or their paramterised equivalents **TEST_VALUES** or **TEST_F_VALUES** repsectively.

The test body should contain one or more expectations, and can contain any other logic required. All the expectations in a test must evaluate to true for the test to pass. Exceptions thrown within the test body will fail the test, although any expectations passed until that point will contribute to the pass totals.

The test runner identifies individual tests by a combination of group name and test name. Not assigning a group is valid, however every test must have a name. For this reason test uniqueness is defined solely by the group and name values supplied to the macro, and while test code/objects can be scoped via namespaces, the test runner will ignore only uniquely identify via it's group and name. Valid names must follow C++ syntax rules for typedef naming.

> The Test runner identified tests base on their group and name are uniquness cannot be gained by scoping tests.

To create a test program, simply delcare a single test runner main function.

    JAHOUTF_TEST_RUNNER { RUNALL }

The main block can contain any user code required but must declare either *RUNALL* (or *SHUFFLE*) in order to actually invoke tests. This is essentially the main entry for the program and only this is required (no other main function should be used or implemented when using this macro). Tests will be automaticaly picked up and installed for use in the test runner.

SECTION tests can be implented as follows:

    SECTION(group, name)
    {
        // Test body
    }

Unlike test cases, section tests are contained within scoped body of existing code (i.e your program). They simply execute the code block and recorde the results which can be later reported. Since the test runner is not used, the group and name of the section are used only for collation and reporting of the results. This also means sections can reuse groups and names within the samme session.

To use *SECTION* test, the jahoutf instance needs to be instantiated so that a session can recod

    JAHOUT_INSTANCE




# Test Case
A test case can have 0 or more expectations, all of which must assert true for the test to pass.

    TEST(isOdd)
    {
        int x = 1;
        EXPECT_EQ(x % 2, 1)
    }


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

* Can test global functions
* Can test exceptions
* Can test anything instantiated or called in the test body.
* Can be nested in other namespaces 





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











