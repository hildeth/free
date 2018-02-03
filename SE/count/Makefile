all: run

run: SEN
	./SEN

SEN: SEN.cpp BFS.cpp | *.hpp
	g++ -std=c++11 -o $@ $^
