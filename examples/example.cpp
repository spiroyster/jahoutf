// Only need to include jahoutf.hpp
#include "../include/LogiQA.hpp"

// Use the test runner to run all the tests...
LOGIQA_TEST_RUNNER { RUNALL }

// A single test with a single assertion that passes...
LOGIQA_TEST(helloWorld, "") { ASSERT_PASS }
