// THHSolver.h
//
// Tom's sudoku solver.
//

#include "SudokuGrid.h"
#include <utility>

class THHSolver
{
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
	Set row_set_union(unsigned row, Set perm);
	bool update_row(unsigned row, Set available, Set members);

	bool constrain_columns();
	bool constrain_column(unsigned column);
	Set_pair find_column_closure(unsigned column, Set seen);
	Set column_set_union(unsigned column, Set perm);
	bool update_column(unsigned column, Set available, Set members);

	bool constrain_boxes();
	bool constrain_box(unsigned box);
	Set_pair find_box_closure(unsigned box, Set seen);
	Set box_set_union(unsigned box, Set perm);
	bool update_box(unsigned box, Set available, Set members);
};
