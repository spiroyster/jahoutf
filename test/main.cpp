//#include "../include/LogiQA.hpp"
#include <LogiQA.hpp>

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
