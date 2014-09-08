#include "Set.h"

#include <iostream>

class SudokuGrid
{
  public:

	// Iterates over the elements in the selected row.
	class row_iterator
	{
		SudokuGrid& _base;
		unsigned _i, _j;
		row_iterator() = delete;
	  public:
		~row_iterator() {}
		row_iterator(const row_iterator& rhs)
			: _base(rhs._base), _i(rhs._i), _j(rhs._j) {}
		row_iterator(SudokuGrid& grid, unsigned row)
			: _base(grid), _i(row), _j(1) {}
		bool more() { return _j < 10; }
		Set& operator*() { return _base.at(_i, _j); }
		void operator++() { ++_j; }
		// We don't test _base or _i.
		// Comparing iterators across instances or rows will yield false
		// positives.
		bool operator==(const row_iterator& other)
		{ return _j == other._j; }
	};
	// Iterates over the elements in the selected column.
	class column_iterator
	{
		SudokuGrid& _base;
		unsigned _i, _j;
		column_iterator() = delete;
	  public:
		~column_iterator() {}
		column_iterator(const column_iterator& rhs)
			: _base(rhs._base), _i(rhs._i), _j(rhs._j) {}
		column_iterator(SudokuGrid& grid, unsigned column)
			: _base(grid), _i(1), _j(column) {}
		bool more() const { return _i < 10; }
		Set& operator*() { return _base.at(_i, _j); }
		void operator++() { ++_i; }
		// We don't test _base or _j.
		// Comparing iterators across instances or columns will yield false
		// positives.
		bool operator==(const column_iterator& other)
		{ return _i == other._i; }
	};
	// Iterates over the elements in the selected square.
	// Squares are numbered in lexical order:
	// 1 2 3
	// 4 5 6
	// 7 8 9
	class box_iterator
	{
		SudokuGrid& _base;
		unsigned _i, _j;
		unsigned _count;
		box_iterator() = delete;
	  public:
		~box_iterator() {}
		box_iterator(const box_iterator& rhs)
			: _base(rhs._base), _i(rhs._i), _j(rhs._j), _count(rhs._count) {}
		box_iterator(SudokuGrid& grid, unsigned box)
			: _base(grid), _count(1)
		{
			_i = 1 + ((box - 1) / 3) * 3;
			_j = 1 + ((box - 1) % 3) * 3;
		}
		bool more() { return _count < 10; }
		Set& operator*() { return _base.at(_i, _j); }
		void operator++()
		{
			++_count;
			if (_j % 3 == 0) { _j -= 2; ++_i; }
			else ++_j;
		}
		// We don't test _base or _count.
		// Comparing iterators across instances will yield false
		// positives.
		bool operator==(const box_iterator& other)
		{ return _i = other._i && _j == other._j; }
	};
  private:
	// Represents the possible entries at each location in a sudoku grid.
	// If the set contains a single true bit, that square has been resolved.
	// The grid is made 10x10 so we can use one-based indexing without having to
	// translate indices.
	Set _grid[10][10];

  protected:
	void init_copy(const SudokuGrid& rhs);

  public:
	~SudokuGrid() {}
	SudokuGrid() {}
	SudokuGrid(const SudokuGrid& rhs) { init_copy(rhs); }
	void operator=(const SudokuGrid& rhs) { init_copy(rhs); }

    Set& at(unsigned row, unsigned column) { return _grid[row][column]; }
	// Access an element using box coordinates.
	Set& at_box(unsigned box, unsigned index);
	row_iterator row_begin(unsigned row) { return row_iterator(*this, row); }
	column_iterator column_begin(unsigned column)
	{ return column_iterator(*this, column); }
	box_iterator box_begin(unsigned box) { return box_iterator(*this, box); }
	void map_row(unsigned row, Set::set_op op);
	void map_column(unsigned column, Set::set_op op);
	void map_box(unsigned box, Set::set_op op);

	friend std::ostream& operator<<(std::ostream& os, SudokuGrid& grid);
	friend std::istream& operator>>(std::istream& is, SudokuGrid& grid);

	void debug_print();
};
