#ifndef LOGIQA_FIOS_HPP
#define LOGIQA_FIOS_HPP

#include "..\include\LogiQA.hpp"
#include <iostream>
#include <fstream>

namespace logiqa
{
    
struct cmd_result
{
    cmd_result(const std::string& o, const std::string& e, int status)
    :   out_(o), err_(e), status_(status)
    {
    }
    std::string out_;
    std::string err_;
    int status_;
};

static void file_write(const std::string& filename, const std::string& content)
{
    std::ofstream file(filename.c_str());
    if (!file)
		throw std::runtime_error("Unable to open file for output " + filename);
	file << content;
}

static std::string file_read(const std::string& filename)
{
    std::ifstream file(filename.c_str());
	if (!file)
		throw std::runtime_error("Unable to open file for input " + filename);
	std::ostringstream syntax;
	syntax << file.rdbuf();
	return syntax.str();
}

static cmd_result invoke_cmd(const std::string& cmd, const std::string& args, const std::string& out_file, const std::string& err_file)
{
	std::string cmd_to_call = logiqa::session().test_runner_path_ + cmd + " " + args + " > " + out_file +" 2> " + err_file;
    const auto status = std::system(cmd_to_call.c_str());
    if (status != 0)
        throw std::runtime_error("Cmd returned status " + std::to_string(status));
    return cmd_result(file_read(out_file), file_read(err_file), status);
}

static std::string expected_path(const std::string& name)
{
#ifdef LOGIQA_NIX
    return logiqa::session().test_runner_path_ + "expected/" + name;
#endif
#ifdef LOGIQA_WIN
    return logiqa::session().test_runner_path_ + "expected\\" + name;
#endif
}

static std::string result_path(const std::string& name)
{
#ifdef LOGIQA_NIX
    return logiqa::session().test_runner_path_ + "result/" + name;
#endif
#ifdef LOGIQA_WIN
    return logiqa::session().test_runner_path_ + "result\\" + name;
#endif
}

static cmd_result invoke_and_record(const std::string& cmd, const std::string& args, const std::string& test_name)
{
    auto result = invoke_cmd(cmd, args, result_path("std_out"), result_path("std_err"));

    if (!result.err_.empty())
        file_write(result_path(test_name), result.err_);
    else
        file_write(result_path(test_name), result.out_);

    return result;
}

}


#endif // LOGIQA_FIOS_HPP