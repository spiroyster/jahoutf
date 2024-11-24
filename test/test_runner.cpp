#include "..\include\LogiQA.hpp"

#include "fios.hpp"

// Test runner invokes, and checks output...
LOGIQA_TEST(test_runner_no_tests, "test_runner should_pass")  
{ 
    auto result = invoke_and_record("test_stub", "x", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_all_tests, "test_runner should_pass") 
{ 
    auto result = invoke_and_record("test_stub", "", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_about, "test_runner should_pass") 
{ 
    auto result = invoke_and_record("test_stub", "?", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_list, "test_runner should_pass") 
{ 
    auto result = invoke_and_record("test_stub", "-list", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_tags, "test_runner should_pass") 
{ 
    auto result = invoke_and_record("test_stub", "-tags", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_list_tags, "test_runner should_pass") 
{ 
    auto result = invoke_and_record("test_stub", "-list -tags", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_pass, "test_runner should_pass") 
{ 
    auto result = invoke_and_record("test_stub", "pass", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_fail, "test_runner should_pass") 
{ 
    auto result = invoke_and_record("test_stub", "fail", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_exception, "test_runner should_pass") 
{ 
    auto result = invoke_and_record("test_stub", "exception", logiqa_name());
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
