rem @echo off

rem set the vars and paths...
set thisPath=%~dp0
set exePath=%thisPath%..\msvc\x64\Debug\

rem Prep the results dir...
rmdir /S /Q %thisPath%expected\win
mkdir %thisPath%expected\win

rem Generate the output from the test stub...
%exePath%test_stub.exe x > %thisPath%expected\win\test_runner_no_tests
%exePath%test_stub.exe > %thisPath%expected\win\test_runner_all_tests
%exePath%test_stub.exe ? > %thisPath%expected\win\test_runner_about
%exePath%test_stub.exe pass > %thisPath%expected\win\test_runner_pass
%exePath%test_stub.exe fail > %thisPath%expected\win\test_runner_fail
%exePath%test_stub.exe exception > %thisPath%expected\win\test_runner_exception
%exePath%test_stub.exe -list > %thisPath%expected\win\test_runner_list
%exePath%test_stub.exe -list -tags > %thisPath%expected\win\test_runner_list_tags
%exePath%test_stub.exe -tags > %thisPath%expected\win\test_runner_tags

rem Generate the output for the individual pass, fails, and exceptions...
%exePath%test_stages.exe should_pass > %thisPath%expected\win\should_pass
%exePath%test_stages.exe should_fail > %thisPath%expected\win\should_fail
%exePath%test_stages.exe should_throw > %thisPath%expected\win\should_throw

rem copy the contents to the exe path...
xcopy /s /i /y %thisPath%expected\win\* %exePath%expected
