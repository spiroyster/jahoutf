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



void file_write(const std::string& filename, const std::string& content)
{
    std::ofstream file(filename.c_str());
    if (!file)
		throw std::runtime_error("Unable to open file for output " + filename);
	file << content;
}

std::string file_read(const std::string& filename)
{
    std::ifstream file(filename.c_str());
	if (!file)
		throw std::runtime_error("Unable to open file for input " + filename);
	std::ostringstream syntax;
	syntax << file.rdbuf();
	return syntax.str();
}

cmd_stub_result invoke(const std::string& cmd, const std::string& args, const std::string& out_file, const std::string& err_file)
{
	std::string cmd_to_call = logiqa::session().test_runner_path_ + "test_stub " + args + " > " + out_file +" 2> " + err_file;
    const auto status = std::system(cmd_to_call.c_str());
    if (status != 0)
        throw std::runtime_error("Cmd returned status " + std::to_string(status));
    return cmd_stub_result(file_read(out_file), file_read(err_file), status);
}

std::string expected_path(const std::string& name)
{
#ifdef LOGIQA_NIX
    return logiqa::session().test_runner_path_ + "expected/" + name;
#endif
#ifdef LOGIQA_WIN
    return logiqa::session().test_runner_path_ + name + "\\";
#endif
}

std::string result_path(const std::string& name)
{
#ifdef LOGIQA_NIX
    return logiqa::session().test_runner_path_ + "result/" + name;
#endif
#ifdef LOGIQA_WIN
    return logiqa::session().test_runner_path_ + name + "\\";
#endif
}


// Test runner invokes, and checks output...
LOGIQA_TEST(test_runner_no_tests, "test_runner should_pass")  
{ 
    auto result = invoke("test_stub", "x", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}

LOGIQA_TEST(test_runner_all_tests, "test_runner should_pass") 
{ 
    auto result = invoke("test_stub", "", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_about, "test_runner should_pass") 
{ 
    auto result = invoke("test_stub", "?", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_list, "test_runner should_pass") 
{ 
    auto result = invoke("test_stub", "-list", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_tags, "test_runner should_pass") 
{ 
    auto result = invoke("test_stub", "-tags", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_list_tags, "test_runner should_pass") 
{ 
    auto result = invoke("test_stub", "-list -tags", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_pass, "test_runner should_pass") 
{ 
    auto result = invoke("test_stub", "pass", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_fail, "test_runner should_pass") 
{ 
    auto result = invoke("test_stub", "fail", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
LOGIQA_TEST(test_runner_exception, "test_runner should_pass") 
{ 
    auto result = invoke("test_stub", "exception", expected_path(logiqa_name()), result_path(logiqa_name()));
    ASSERT_EQ_STR(result.out_, file_read(expected_path(logiqa_name())))
}
