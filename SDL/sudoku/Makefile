SOURCE_NAMES = SudokuGrid THHSolver Set
INCLUDES = $(SOURCE_NAMES:%=%.h)
SOURCES = $(SOURCE_NAMES:%=%.cpp)

app: $(INCLUDES) $(SOURCES)
	g++ -g -std=gnu++0x -o app main.cpp $(SOURCES)

tests:
	cd tests; make