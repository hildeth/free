all: lib

lib: classes.a

OBJNAMES = Tag Full
OBJS = $(OBJNAMES:%=%.o)

CPPOPTS = -std=c++11

classes.a: $(OBJS)
	ar crs $@ $?

.cpp.o:
	g++ $(CPPOPTS) -c $<

