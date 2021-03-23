#include "jahoutf.hpp"

JAHOUTF_INSTANCE

int main(int argc, char** argv)
{
    int x = 2;
    int y = 1;

    TEST_S(testgroup, add)
    {
        EXPECT_EQ(x+y, 3)
    }
    TEST_S(testgroup, subtract)
    {
        EXPECT_EQ(x-y, 1)
    }

    // Manually post results, alternatively jahoutf instance will report results on program destruction.

    return 0;
}