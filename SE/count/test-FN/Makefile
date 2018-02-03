all: test

test: test-FN
	./test-FN

SOURCES:=test.cpp ../BFS.cpp
DEPS:=../FN.hpp
OBJDIR:=objs

test-FN: $(SOURCES) | $(DEPS)
	g++ -std=c++11 -I.. -o $@ $^

