#include "../include/LogiQA.hpp"

class foo : public logiqa::fixture { public: int x_ = 42; };

LOGIQA_TEST_FIXTURE(single_fixture, "fixture should_pass", foo) { ASSERT_EQ(x_, 42) }

class foo_setup : public logiqa::fixture 
{
public:
    void setup() { x_ = 42; }
    int x_ = 0;
};

LOGIQA_TEST_FIXTURE(single_fixture_setup, "fixture should_pass", foo_setup) { ASSERT_EQ(x_, 42) }

class foo_method : public logiqa::fixture
{
public:
    int method(int v) { return x_ + v; }
    int x_ = 42;
};

LOGIQA_TEST_FIXTURE(single_fixture_method, "fixture should_pass", foo_method) { ASSERT_EQ(method(0), 42) }

logiqa::param<int> fixture_ints({ 0, 1, 2, 3, 4, 5 });

LOGIQA_TEST_FIXTURE_PARAMS(single_fixture_method_params, "fixture should_pass params", foo_method, fixture_ints)
{
    ASSERT_EQ(method(logiqa_param()), logiqa_param() + 42)
}

class foo_params_setup : public logiqa::fixture_param<int>
{
public:
    void setup()
    {
        x_ = logiqa_param();
    }
    int x_;
};

LOGIQA_TEST_FIXTURE_PARAMS(fixture_params_setup, "fixture params should_pass", foo_params_setup, fixture_ints)
{
    ASSERT_EQ(x_, logiqa_param());
}

class foo_params_method : public logiqa::fixture_param<int>
{
public:
    int method(int v)
    {
        return v + logiqa_param();
    }
};

LOGIQA_TEST_FIXTURE_PARAMS(fixture_params_method, "fixture params should_pass", foo_params_method, fixture_ints)
{
    ASSERT_EQ(method(logiqa_param()), logiqa_param() * 2)
}

// Exceptions...
class foo_exception_setup : public logiqa::fixture
{
public:
    void setup() { throw std::runtime_error("Setup exception."); }
};

LOGIQA_TEST_FIXTURE(setup_exception, "fixture should_throw", foo_exception_setup) { ASSERT_FAIL }

class foo_exception_teardown : public logiqa::fixture
{
public:
    void teardown() { throw std::runtime_error("Teardown exception."); }
};

LOGIQA_TEST_FIXTURE(teardown_exception, "fixture should_throw", foo_exception_teardown) { ASSERT_PASS }