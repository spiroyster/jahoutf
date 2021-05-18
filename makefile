CXX=g++

test: ./test/main.cpp ./test/assertions.cpp
	mkdir -p bin
	$(CXX) test/main.cpp test/assertions.cpp -o bin/test

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