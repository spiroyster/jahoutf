#!/bin/bash

rm -r ./expected/linux
mkdir ./expected/linux
../bin/test_stub x > expected/linux/test_runner_no_tests
../bin/test_stub > expected/linux/test_runner_all_tests
../bin/test_stub ? > expected/linux/test_runner_about
../bin/test_stub pass > expected/linux/test_runner_pass
../bin/test_stub fail > expected/linux/test_runner_fail
../bin/test_stub exception > expected/linux/test_runner_exception
../bin/test_stub -list > expected/linux/test_runner_list
../bin/test_stub -list -tags > expected/linux/test_runner_list_tags
../bin/test_stub -tags > expected/linux/test_runner_tags


