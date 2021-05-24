CXX=g++ -g -Iinclude

all: test examples
	
clean:
	rm -r bin
	mkdir -p bin
	
test_install:
	mkdir -p bin/result
	mkdir -p bin/expected
	cp test/expected/linux/* bin/expected

test_stages: ./test/test_stages.cpp ./test/assertions.cpp ./test/fixtures.cpp ./test/test_runner.cpp
	$(CXX) test/test_stages.cpp test/assertions.cpp test/fixtures.cpp test/test_runner.cpp -o bin/test_stages

test_stub: ./test/test_stub.cpp
	$(CXX) test/test_stub.cpp -o bin/test_stub

test_main: ./test/test_main.cpp
	$(CXX) test/test_main.cpp -o bin/test

test: test_main test_stages test_stub test_install
	
example: ./examples/example.cpp
	$(CXX) examples/example.cpp -o bin/example 

example2: ./examples/example2.cpp
	$(CXX) examples/example2.cpp -o bin/example2 

examples: example example2



	