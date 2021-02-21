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

// class myFixtureWithParams : public jahoutf::test_param<int>
// {

// };

// TEST_P(myfixtureWithParams, theTestName)
// {

// }

// INSTANTIATE_TEST_CASE_P(
//         LeapYearTests,
//         LeapYearParameterizedTestFixture,
//         ::testing::Values(
//                 1, 711, 1989, 2013
//         ));


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
        unsigned int x_, y_;
};

// TEST_P, testfixtureparam, testgroup, testname
namespace testgroup
{
    class testname : public myFixtureWithParam
    {
    public:
        testname(jahoutf::test* t, unsigned int n)
        {
            // setup param information...
            jahoutf_current_test_ = t;
            param_id_ = n;
        }
        void jahoutf_test_body();
    };
}
void testgroup::testname::jahoutf_test_body()
{
    // test code...
}


// INSTANTIATE_TEST_CASE_P(test_group, testname, TEST_P testname, jahoutf::values<int>({}))
namespace testgroup
{
    class param_test : public jahoutf::test
    {
        std::vector<int> params_ = { 21, 22, 23, 24, 25 };
    public:
        param_test() 
        : jahoutf::test("testgroup", "testname[]") 
        { 
            jahoutf::session().tests_["testgroup"].push_back(this); 
        }
        void jahoutf_test_invoke()
        {
            // for each param...
            for (unsigned int p = 0; p < params_.size(); ++p)
            {
                // create the wrapper version, set the param...
                try
                {
                    testname w(this, p);
                    w.param_ = params_[p]; 
                    current_fixture_ = &w;
                    try
                    { 
                        current_fixture_->Setup();
                        test::jahoutf_test_invoke(); 
                        try { current_fixture_->TearDown(); } 
                        JAHOUTF_CATCH_EXCEPTION(fixture_teardown)
                    }
                    JAHOUTF_CATCH_EXCEPTION(fixture_setup)
                }
                JAHOUTF_CATCH_EXCEPTION(fixture_construct)
            }
        }
        void jahoutf_test_body() { current_fixture_->jahoutf_test_body(); }
        testname* current_fixture_;
    };
}

