all: \
test

test: test1

test1: test-Butterfly
	./test-Butterfly

test-Butterfly: Butterfly.cpp ../Butterfly.hpp
	g++ -std=c++11 -o $@ $^

