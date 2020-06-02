all: tests

tests: tests.cpp bst.h heap.h
	g++ tests.cpp -o tests -std=c++17