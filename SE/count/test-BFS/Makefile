all: test

test: test-BFS
	./test-BFS

test-BFS: test.cpp ../BFS.hpp
	g++ -std=c++11 -I.. -o $@ $^

