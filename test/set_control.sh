#!/bin/bash

# Prep the results dir...
rm -r ./expected/linux
mkdir ./expected/linux

# Generate the output from the test stub...
../bin/test_stub x > expected/linux/test_runner_no_tests
../bin/test_stub > expected/linux/test_runner_all_tests
../bin/test_stub ? > expected/linux/test_runner_about
../bin/test_stub pass > expected/linux/test_runner_pass
../bin/test_stub fail > expected/linux/test_runner_fail
../bin/test_stub exception > expected/linux/test_runner_exception
../bin/test_stub -list > expected/linux/test_runner_list
../bin/test_stub -list -tags > expected/linux/test_runner_list_tags
../bin/test_stub -tags > expected/linux/test_runner_tags

# Generate the output for the individual pass, fails, and exceptions...
../bin/test_stages should_pass > expected/linux/should_pass
../bin/test_stages should_fail > expected/linux/should_fail
../bin/test_stages should_throw > expected/linux/should_throw
