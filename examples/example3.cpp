//#include "../include/LogiQA.hpp"
//
//LOGIQA_INSTANCE
//
//int main(int argc, char** argv)
//{
//    int x = 2;
//    int y = 1;
//
//    class testSection1 : public logiqa::test
//    {
//    public:
//        testSection1(const std::string& name)
//            :   logiqa::test(name, 0, "")
//        {
//        }
//
//        // The test body that is invoked.
//        virtual void logiqa_body() {}
//
//    };
//    
//    if (auto t = std::make_shared<testSection1>("name"))
//    {
//
//    }
//            
//
//   /* TEST_S(testgroup, add)
//    {
//        EXPECT_EQ(x+y, 3)
//    }
//    TEST_S(testgroup, subtract)
//    {
//        EXPECT_EQ(x-y, 1)
//    }*/
//
//    // Manually post results, alternatively jahoutf instance will report results on program destruction.
//    //JAHOUTF_POST
//
//    return 0;
//}
