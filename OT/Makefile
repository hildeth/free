PROG:=rv_test

CPPFLAGS:=--std=c++11
CPPFLAGS+=-DUNIT_TEST

SRCS:=RealVector.cpp
HDRS:=$(SRCS:%.cpp=%.hpp)
OBJS:=$(SRCS:%.cpp=%.o)

run: $(PROG)
	./$(PROG)

$(PROG): $(OBJS)
	g++ $(CPPFLAGS) $< -o $@

%.o: %.cpp %.hpp
	g++ $(CPPFLAGS) -c $<

