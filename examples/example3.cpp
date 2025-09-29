#include "../include/jahoutf.hpp"

// Test examples showing how to use simple tests, fixtures, parameterised tests (values) 

class console : public jahoutf::console
{
public:
	void suite_start(const jahoutf::test_list& tests)
	{
		
	}
	void suite_end(const jahoutf::test_list& tests, const jahoutf::summary& summary)
	{

	}

};

// Use the in-built test runner to run all the tests...
JAHOUTF_TEST_RUNNER
{ 
	JAHOUTF_EVENT(console)
	RUNALL 
}



