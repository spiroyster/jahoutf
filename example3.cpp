#include "jahoutf.hpp"

JAHOUTF_INIT


int main(int argc, char** argv)
{
    int x = 2;
    int y = 1;

    TEST_SECTION(testgroup, add)
    {
        EXPECT_EQ(x+y, 3)
    }
    TEST_SECTION(testgroup, subtract)
    {
        EXPECT_EQ(x-y, 1)
    }

    return 0;
}