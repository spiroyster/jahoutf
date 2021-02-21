#include "jahoutf.hpp"

// Introspective testing...
JAHOUTF_MAIN { RUNALL }

TEST(shouldpass, success) { SUCCESS }
TEST(shouldpass, expect) { EXPECT(true) }
TEST(shouldpass, expect_eq) { EXPECT_EQ(1,1) }

TEST(shouldfail, success) { FAIL }
TEST(shouldfail, expect) { EXPECT(false) }
TEST(shouldfail, expect_eq) { EXPECT_EQ(1,2) }

TEST(exceptiontest, testcase_stdexception) { throw std::runtime_error("Std exception throw test"); }
