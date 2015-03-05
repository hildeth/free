all: app

clean:
	rm $(OBJS)

lib: classes.a

app: driver

CXXFLAGS = -g -std=c++11

driver: driver.cpp classes.a
	g++ $(CXXFLAGS) -o driver driver.cpp classes.a

OBJNAMES = Tag Full
OBJS = $(OBJNAMES:%=%.o)

classes.a: $(OBJS)
	ar crs $@ $?

.cpp.o:
	g++ $(CXXFLAGS) -c $<

