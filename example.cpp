#include "jahoutf.hpp"

// Declare test program main, and run all the tests.
JAHOUTF_TEST_RUNNER { RUNALL }


struct testdata
{
    std::string str_;
    std::string pattern_;
    bool expected_;
};

// // Test the str matching...
// TEST_VALUES(patternmatching, jahoutf::values<testdata>({ { "abcdefghijlklmnopqrstuvwxyz", "a*", true },
//  { "abcdefghijlklmnopqrstuvwxyz", "*a*", true },
//  { "abcdefghijlklmnopqrstuvwxyz", "**a*", true },
//  { "abcdefghijlklmnopqrstuvwxyz", "a**", true },
//  { "abcdefghijlklmnopqrstuvwxyz", "a", false }
//  }))
// {
//     auto p = jahoutf_value();
//     EXPECT(jahoutf::_::pattern_match(p.str_, p.pattern_))
// }