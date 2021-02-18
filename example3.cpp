#include "jahoutf.hpp"

JAHOUTF_INIT

namespace jahoutf
{
    
}
void test_case_report_success(const std::string& file, unsigned int lineNum, const std::string& msg = "") 
{
    //if (test_)
     //   test_->test_case_report_success(file, lineNum, msg);
}
void test_case_report_fail(const std::string& file, unsigned int lineNum, const std::string& msg = "") 
{
    //if (test_)
     //   test_->test_case_report_fail(file, lineNum, msg);
}
void test_case_report_thrown(const std::string& location, const std::string& e) 
{
    //if (test_)
     //   test_->test_case_report_thrown(location, e);
}

int main(int argc, char** argv)
{
    int x = 2;
    int y = 1;

    {
        EXPECT_EQ(x+y, 3)
    }

    return 0;
}