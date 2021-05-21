CXX=g++ -Iinclude

test: ./test/main.cpp ./test/assertions.cpp ./test/fixtures.cpp ./test/test_runner.cpp
	mkdir -p bin
	$(CXX) test/main.cpp test/assertions.cpp test/fixtures.cpp test/test_runner.cpp -o bin/test
	$(CXX) test/stub.cpp -o bin/stub

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