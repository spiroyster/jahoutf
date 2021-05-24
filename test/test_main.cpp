#include <LogiQA.hpp>
#include "fios.hpp"

class custom_output_message : public logiqa::console
{
public:
    void suite_end(const logiqa::test_list& tests_to_run, const logiqa::results::summary& summary)
    {
        logiqa::console::suite_end(tests_to_run, summary);
        message("\nThis test will include failing tests and exceptions.\n"); 
        message("To only show the passing tests, run tag \"should_pass\" ...\n");
        message("To only show the failing tests, run tag \"should_fail\" ...\n");
        message("To only show the throwing, run tag \"should_throw\" ...\n");
    }
};

// Introspective testing...
LOGIQA_TEST_RUNNER 
{
	EVENT(custom_output_message)
    RUNALL 
}

LOGIQA_TEST(should_pass, "")
{
    auto result = invoke_and_record("test_stages", "should_pass", logiqa_name());
    ASSERT_EQ(result.out_.compare(file_read(expected_path(logiqa_name()))), 0)
}
LOGIQA_TEST(should_fail, "")
{
    auto result = invoke_and_record("test_stages", "should_fail", logiqa_name());
    ASSERT_EQ(result.out_.compare(file_read(expected_path(logiqa_name()))), 0)
}
LOGIQA_TEST(should_throw, "")
{
    auto result = invoke_and_record("test_stages", "should_throw", logiqa_name());
    ASSERT_EQ(result.out_.compare(file_read(expected_path(logiqa_name()))), 0)
}

