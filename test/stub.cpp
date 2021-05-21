#include <LogiQA.hpp>

LOGIQA_TEST_RUNNER { RUNALL }


LOGIQA_TEST(pass, "pass")
{
    ASSERT_PASS
}

LOGIQA_TEST(fail, "fail")
{
    ASSERT_FAIL
}

LOGIQA_TEST(halt, "exception")
{
    TEST_HALT
}

LOGIQA_TEST_PARAMS(params, "params pass", logiqa::param<int>({ 0, 1, 2, 3, 4 }))
{
    ASSERT_PASS
}