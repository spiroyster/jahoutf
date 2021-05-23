CXX=g++ -g -Iinclude

test: ./test/main.cpp ./test/assertions.cpp ./test/fixtures.cpp ./test/test_runner.cpp
	mkdir -p bin/result
	mkdir -p bin/expected
	cp -r test/expected/linux/* bin/expected
	$(CXX) test/main.cpp test/assertions.cpp test/fixtures.cpp test/test_runner.cpp -o bin/test
	$(CXX) test/test_stub.cpp -o bin/test_stub

example: ./examples/example.cpp
	mkdir -p bin
	$(CXX) examples/example.cpp -o bin/example 

example2: ./examples/example2.cpp
	mkdir -p bin
	$(CXX) examples/example2.cpp -o bin/example2 

examples: example example2

all: examples test

clean:
	rm -r bin