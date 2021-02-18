#include "jahoutf.hpp"

JAHOUTF_MAIN { RUNALL }

TEST(add)
{
    int x = 1, y = 2;
    EXPECT_EQ(x + y, 3)
}

TEST(subtract)
{
    int x = 1, y = 2;
    EXPECT_EQ(x - y, -1)
}

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



class myFixture : public jahoutf::test_fixture
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


// // Parameterised tests....
// class myFixtureWithParams : public jahoutf::test_param<int>
// {
//     public:
//     int doSomething(int x, int y)
//     {
//         return x+y;
//     }

// };


// namespace groupname
// {
//     class testname : public jahoutf::test_interface
//     {
//         public:
//         class wrapper : public myFixtureWithParams
//         {

//         };

//     };
// }

// TEST_FP(myFixture, fixture3, 
//{


//})

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
