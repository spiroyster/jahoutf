#include <LogiQA.hpp>

#include <iostream>

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
    std::string out_file(logiqa::session().test_runner_path_ + "stub_out");
    std::string err_file(logiqa::session().test_runner_path_ + "stub_err");

	std::string cmd = logiqa::session().test_runner_path_ + "test_stub " + args + " > " + out_file +" 2> " + err_file;
    
    const auto status = std::system(cmd.c_str());

    std::ifstream out_stream(out_file.c_str());
    std::ifstream err_stream(err_file.c_str());

    if (!out_stream)
        throw std::runtime_error("Unable to open " + out_file);
    if (!err_stream)
        throw std::runtime_error("Unable to open " + err_file);
        
	std::ostringstream out_syntax, err_syntax;
	out_syntax << out_stream.rdbuf();
	err_syntax << err_stream.rdbuf();
	
	//err_syntax;
    std::remove(out_file.c_str());
    std::remove(err_file.c_str());

    return cmd_stub_result(out_syntax.str(), err_syntax.str(), status);
}

std::string cmd_stub_expected(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file)
		throw std::runtime_error("Unable to open expected file " + filename);
	std::ostringstream syntax;
	syntax << file.rdbuf();
	return syntax.str();
}

// Test runner invokes, and checks output...
LOGIQA_TEST(test_runner_no_tests, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("x").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
LOGIQA_TEST(test_runner_all_tests, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
LOGIQA_TEST(test_runner_about, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("?").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
LOGIQA_TEST(test_runner_list, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("-list").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
LOGIQA_TEST(test_runner_tags, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("-tags").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
LOGIQA_TEST(test_runner_list_tags, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("-list -tags").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
LOGIQA_TEST(test_runner_pass, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("pass").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
LOGIQA_TEST(test_runner_fail, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("fail").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
LOGIQA_TEST(test_runner_exception, "test_runner should_pass") { ASSERT_EQ_STR(cmd_stub("exception").out_, cmd_stub_expected(session().test_runner_path_ + "expected\\" + logiqa_name())) }
