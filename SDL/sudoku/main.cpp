#include "THHSolver.h"

int main()
{
	SudokuGrid theGrid;
	std::cin >> theGrid;
	THHSolver solver(theGrid);
	solver.solve();
	std::cout << theGrid << std::endl;
	return 0;
}
