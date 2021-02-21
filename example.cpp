#include "jahoutf.hpp"

// Declare test program main, and run all the tests.
JAHOUTF_MAIN { RUNALL }

// Create a test called test1, in the group helloTestWorld which passes.
TEST(helloTestWorld, test1) { SUCCESS }

// Create a test called test2, in the group helloTestWorld which fails.
TEST(helloTestWorld, test2) { FAIL }