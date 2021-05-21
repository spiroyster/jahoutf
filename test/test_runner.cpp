#include <LogiQA.hpp>

#include <iostream>

struct test_runner_case
{
    test_runner_case(const std::string& args, const std::string& expected) : args_(args), expected_(expected) {}
    std::string args_;
    std::string expected_;
};

struct cmd_stub_result
{
    cmd_stub_result(const std::string& o, const std::string& e, int status)
    :   out_(o), err_(e), status_(status)
    {
    }
    std::string out_;
    std::string err_;
    int status_;
};

cmd_stub_result cmd_stub(const std::string& args)
{
    // invoke the process and 
    std::string out_file("stub_out");
    std::string err_file("stub_err");

    std::string cmd = "stub " + args + " > " + out_file + " 2> " + err_file;
    
    const auto status = std::system(cmd.c_str());

    std::ifstream out_stream(out_file.c_str());
    std::ifstream err_stream(err_file.c_str());

    if (!out_stream)
        throw std::runtime_error("Unable to open " + out_file);
    if (!err_stream)
        throw std::runtime_error("Unable to open " + err_file);
        
    std::string out_syntax, err_syntax;
    out_stream >> out_syntax;
    err_stream >> err_syntax;
    std::remove(out_file.c_str());
    std::remove(err_file.c_str());

    return cmd_stub_result(out_syntax, err_syntax, status);
}

logiqa::param<test_runner_case> cases({
    test_runner_case("?", "")
});


// Test runner invokes, and checks output...
LOGIQA_TEST_PARAMS(test_runner_no_tests, "test_runner should_pass", cases)
{
    auto result = cmd_stub(logiqa_param().args_);
    ASSERT_EQ(result.out_.compare(logiqa_param().expected_), 0);
}

// Test run with no tests

// Test run tests

// Test run -list

// Test run -list -tags

// Test run -tags

// Test run about

// Test run "pass"

// Test run "fail"

// Test run "exception"

// Test run silence

// Test run shuffle

// Test run xUnit output...