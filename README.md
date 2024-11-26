# jahoutf
Just another header only unit testing framework... jahoutf is a single header unit testing framework with no dependencies other than C++ STL. There is an in-built test runner which compiles to a stand alone executable, as well as xUnit (XML) compatible output to deliver test results.
Macros are used extensively, and ultimately I would like to make them the same as googletest so, in theory, should be compatible with current googletest tests and can be a drop in replacement. 

### Supported

* Lightweight.
* (Single) Header only.
* STL only.
* xUnit output.
* (In-built) Test runner.
* Fixtures supported.
* Parameterised values suported.
* Customisable output and reporting.
* Test Shuffling.
* Test pattern-matching.

### Not supported

* Mocking
* Section tests (Program in-situ tests)

[Creating Tests](#creating-tests)

[Running Tests](#running-tests)

[Customisation](#customising)

-Z must be used for correct macro expansion with msvc.


## Quick Example
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

    <?xml version="1.0" encoding="UTF-8"?>
    <testsuite name="./example" tests="1" failures="0" disabled="0" errors="0" time="0">
        <testcase name="helloWorld" status="run" time="0" classname="">
        </testcase>
    </testsuite>
    </testsuite>


# Creating Tests

Tests can either be written in a test program (and thus run by the test runner), or they can be written into sections within existing code to run tests in-situ. The syntax for declaring a test and implmenting a test is as follows:

    TEST_x(group, name, ...)
    {
        // Test body
    }

*TEST_x* is a macro that can be **TEST** for test cases, **TEST_F** for test fixtures or their paramterised equivalents **TEST_VALUES** or **TEST_F_VALUES** repsectively.

The test body should contain one or more expectations, and can contain any other logic required. All the expectations in a test must evaluate to true for the test to pass. Exceptions thrown within the test body will fail the test, although any expectations passed until that point will contribute to the pass totals.

The test runner identifies individual tests by a combination of group name and test name. Not assigning a group is valid, however every test must have a name. For this reason test uniqueness is defined solely by the group and name values supplied to the macro rather than program scoping rules. Valid names must follow C++ syntax rules for typedef naming.

To create a test program, simply delcare a single test runner main function.

    JAHOUTF_TEST_RUNNER { RUNALL }

The main block can contain any user code required but must declare either **RUNALL** (or **SHUFFLE**) in order to actually invoke tests. This is essentially the main entry for the program and only this is required (no other main function should be used or implemented when using this macro). Tests will be automaticaly picked up and installed for use in the test runner.

## Test Case
A test case can have zero or more expectations, all of which must assert true for the test to pass.

    TEST(isOdd)
    {
        int x = 1;
        EXPECT_EQ(x % 2, 1)
    }

Test cases provide a safe environment to test global functions, exceptions or anything instantiated or called in the test body. They can be nested in namespaces however the name must unque within the group defined in the macro (if any).

    TEST(isOdd, one)
    {
        int x = 1;
        EXPECT_EQ(x % 2, 1)
    }

    TEST(isEven, one)
    {
        int x = 1;
        EXPECT_EQ(x % 2, 0)
    }

* Test global functions.
* Test exceptions with safe handling.
* Test local object construction and destruction.

## Test Fixture
A fixture is a custom class which provides state to a test case and can use custom setup and teardown routines to prep and cleanup for a test case that uses the fixture. To provide a fixture, the fixture class must derive from jahoutf::test_fixture

    class foo : public jahoutf::fixture
    {
        public:
        bool IsOdd(int v) { return v % 2; }
        bool IsEven(int v) { return !IsOdd(v); }
    };

Then tests can be writtem which accept the fixture using the **TEST_F** macro, and passing the fixture class.

    TEST_F(isOdd, one, foo) { EXPECT(IsOdd(1)); }
    TEST_F(isOdd, two, foo) { EXPECT(IsOdd(2)); }

    TEST_F(isEven, one, foo) { EXPECT(IsEven(1)); }
    TEST_F(isEven, two, foo) { EXPECT(IsEven(2)); }

Fixtures are mutable, although since they are constructed and destructed each test case, longevity is clearly defined and so fixture state is not retained from one test to the next.

Fixtures have *Setup()* and *Teardown()* methods which are invoked immediately before and after the test body and should contain the fixture initialisation and clean-up code.  

    class foo : public jahoutf::fixture
    {
    public:
        std::unique_ptr<obj> x_;
        void Setup()
        {
            x_ = std::make_shared<x>(42);
        }
        void TearDown()
        {
            x_.reset();
        }
    };

Fixture constructor and destructor can be used, however destructors should not throw exceptions so having the clean-up in *TearDown* is advisable. Construction, Setup and Teardown are all safe to throw exceptions and will be trapped and recorded as test failures by jahoutf, however paramterised values are not accessable from the construtor so *Setup()* is advisable for fixture initialisation (see *Test Values* below). 

Fixtures can contain members, methods, other objects etc. The test body inherits access to the fixture object, so fxiture members and methods can be private, however there must be some publicly accessable resources for the test body to access for testing.

* Constructed/Destruted each test case.
* Init/Free code ideally should go in the Setup/TearDown not contructor/destructor.
* Can contain members, methods.
* Test body has 'inherited' access to fixture.
* They are mutable.

## Test Values
Mutiple values can be passed to a test. Values need to be defined in a *jahoutf::values<...>* container.

    auto oddNums = jahoutf::values<int>({ 1, 3, 5, 7, 9 });

And can be passed to a test using the **TEST_VALUES** macro. The value is then accessed within the test body by calling either *jahoutf_param()* or *jahoutf_value()*. 

    TEST_VALUES(isOdd, values, oddNums)
    {
        int x = jahoutf_value();  // or can use jahoutf_param();
        EXPECT_EQ(x % 2, 1)
    }

The values defined in the container are each passed once to the test and are immutable.

## Test Fixture with Values
Values can be used with Fixtures for initialisation and clean-up. Fixtures must derive from *jahoutf::fixture_param<...>* and use the same type as the container.

    class foo_with_fixture : public jahoutf::fixture_param<int>
    {
    public:
        std::unique_ptr<obj> x_;
        void Setup()
        {
            x_ = std::make_shared<x>(jahoutf_param());
        }
        void TearDown()
        {
            x_.reset();
        }
    };

They are accessable from within the *Setup()* and *TearDown()* methods and can be read by calling *jahoutf_param()*. The value is not accesssable in the Fixture constructor or destructor and unlike in the test body, *jahoutf_value()* cannot be used.

The **TEST_F_VALUES** macro can then be used with the *jahoutf::fixture_param<...>* fixture. This macro also supports standard *jahoutf::fixture* fixtures.

    TEST_F_VALUES(isOdd, fixtureParamValues, foo_with_fixture, oddNums)
    {
        EXPECT_EQ(x_ % 2, 1);
    }

    TEST_F_VALUES(isOdd, fixtureValues, foo, oddNums)
    {
        idOdd(jahoutf_value());
    }

* Values are immutable
* Each passed once to the test body.
* Fixtures must derive from jahoutf::fixture_param<> to access the value (not avaliabel in constructor/destructor only setup/teardown)
* Fixtures can only access the param through jahoutf_param();
* Test body can use either jahoutf_param() or jahoutf_value()
* Fixtures must use same type as values type.

# Running Tests
The test runner is compiled as a standalone executable and supports various arguments:

* *-list*  : List all the tests in this program. 
* *-shuffle*  : Shuffles tests in the group before invoking them.
* *-silent*  : Silence the console output.
* *?*  : Display help and information about the test program.
* *-xunit=out.xml*  : Output the results in xUnit xml to the file *out.xml*.

Any other argument passed is assumed to be a pattern to match to decide if to invoke certain tests.

    > ./program isOdd

Runs the test named *isOdd* in the default group only.

    > ./program isOdd.*

Runs all the tests in the *isOdd* group only. 

    > ./program *.one

Runs all the tests in any group called *one*.

Custom code for setting up and cleaning up the global environment can be done in the **JAHOUTF_TEST_RUNNER** body.

    JAHOUTF_TEST_RUNNER
    {
        // custom code before tests are run...
        RUNALL      // either RUNALL or SHUFFLE must be called to run the tests
        // custom code after tests are run and before reported...
    }

# Customising
There are two interfaces avaliable to allow customisation. These are *jahoutf::event_interface* for recieving notifications of test invocation events and expectation events, and *jahoutf::report_interface* which allows user modules to generate reports of the tests. 

Two default modules are provided which are *jahoutf::console* (used by default, removed if *-silent* argument is used) and *jahoutf::xUnit* which generates xUnit xml file (used by provided *-xUnit=file.xml* argument and filename) and these should give an example of interface implementations.

## Event
The *jahoutf::event_interface* provides methods which are called at various stages in the test runner session.
The following example derives from the included *console* module (saves implementing all the methods) and simply adds a welcome message, exit message, and outputs a custom message when a test fails.

    class custom_event : public jahoutf::console
    {
    public:
        void case_fail(const test& test, const std::string& filename, unsigned int lineNum, const std::string& msg) 
        {
            message("Uh-oh" + test.jahoutf_name() " failed.");
            console::case_fail(test, filename, lineNum, msg);
        }
        void suite_start() 
        {
            message("Welcome to another test program!"); console::suite_end();
        }
        void suite_end() 
        {
            console::suite_end();
            if (jahoutf::session().summary_.test_failures != 0)
                message("One or more test failure.. defcon 1");
            else
                message("Good work!");
        }
    };

And can be installed in a test runner program as follows:

    JAHOUTF_TEST_RUNNER
    {
        JAHOUTF_EVENT(custom_event)
        RUNALL
    }

OR at any time using the **JAHOUTF_EVENT** macro if using *Section* tests. Only one event module can be installed at any time.

## Report
The *report_interface* provides a single entry point that is called when the tests have finished. the following example generates csv syntax and outputs to a file.

    class custom_report : jahoutf::report_interface
    {
    public:
        std::string filenane("output.csv");
        void report()
        {
            std::ofstream file(filename);
            for (auto r = session().results_.begin(); r != session().results_.end(); ++r)
                file << r->name << ", " << r->group << ", " << r->success.size() << ", " << r->failure.size() << "\n";
        }
    };

And can be installed in a test runner program as follows:

    JAHOUTF_TEST_RUNNER
    {
        JAHOUTF_REPORT(custom_report)
        RUNALL
    }

Or at any time using the **JAHOUTF_REPORT** macro if using *Section* tests. Unlike *event_interface* modules, there is no limit on how many *report_interface* modules can be installed. 

Happy Testing!






