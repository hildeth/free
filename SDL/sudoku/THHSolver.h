// THHSolver.h
//
// Tom's sudoku solver.
//

#include "SudokuGrid.h"
#include <utility>

class THHSolver
{
	typedef std::pair<Set,Set> Set_pair;
	typedef SudokuGrid::Group Group;

  private:
	SudokuGrid& _grid;
  public:
	~THHSolver() {}
  private:
	THHSolver() = delete;
	THHSolver(const THHSolver&) = delete;
	void operator=(const THHSolver&) = delete;
  public:
	THHSolver(SudokuGrid& grid)
		: _grid(grid) {}
	void solve();
  protected:
	unsigned constrain_all(Group group);
	unsigned constrain_one(Group group, unsigned index);
	Set_pair find_closure(Group group, unsigned index, Set seen);
	Set set_union(Group group, unsigned index, Set perm);
	unsigned update(Group group, unsigned index, Set available, Set members);
};
