.PHONY: all clean

CMD := g++ -std=c++17

all: tests time_tests

tests: tests.cpp bst.h heap.h
	$(CMD) tests.cpp -o tests

time_tests: time_tests.cpp bst.h heap.h
	$(CMD) time_tests.cpp -o time_tests

clean:
	rm tests time_tests