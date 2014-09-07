// THHSolver.h
//
// Tom's sudoku solver.
//

#include "SudokuGrid.h"
#include <utility>

class THHSolver
{
	typedef SudokuGrid::Set Set;
	typedef std::pair<Set,Set> Set_pair;
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
	bool constrain_rows();
	bool constrain_row(unsigned row);
	Set_pair find_row_closure(unsigned row, Set seen);
	Set_pair find_row_closure_size(unsigned row, Set seen, unsigned size);
	bool update_row(unsigned row, Set available, Set members);

	bool constrain_columns();
	bool constrain_column(unsigned column);

	bool constrain_boxes();
	bool constrain_box(unsigned box);
};
