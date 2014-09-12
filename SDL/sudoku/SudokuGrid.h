#include "Set.h"

#include <iostream>

class SudokuGrid
{
  public:
	enum Group {ROW, COLUMN, BOX};

	// Iterates over the elements in the selected group (e.g. ROW 3).
	class iterator
	{
	  protected:
		// A private reference to the thing being iterated.
		SudokuGrid& _grid;
		// We could use pointers and avoid having to store a reference to the grid, and
		// also the iterators might be more efficient -- but harder to code and
		// understand.

		// Which group of indices we're iterating.
		Group _group;

		// The major and minor indices.
		unsigned _major, _minor;

	  private:
		iterator() = delete;

	  public:
		~iterator() {}
	    iterator(SudokuGrid& grid, Group group, unsigned major)
			: _grid(grid), _group(group)
			, _major(major), _minor(1) {}
		iterator(const iterator& rhs)
			: _grid(rhs._grid), _group(rhs._group)
			, _major(rhs._major), _minor(rhs._minor) {}

		// Accessors
		Group group() const { return _group; }
		unsigned group_index() const { return _major; }
		unsigned index() const { return _minor; }
		bool more() const { return _minor < 10; }
		Set& operator*() { return _grid.at(_group, _major, _minor); }
		void operator++() { ++_minor; }
		bool operator==(const iterator& other)
		{ return (_group == other._group &&
				  _major == other._major && _minor == other._minor); }
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
	Set& at(Group group, unsigned group_index, unsigned index);
	
	iterator begin(Group group, unsigned group_index)
	{ return iterator(*this, group, group_index); }
	iterator row_begin(unsigned row)
	{ return iterator(*this, ROW, row); }
	iterator column_begin(unsigned column)
	{ return iterator(*this, COLUMN, column); }
	iterator box_begin(unsigned box)
	{ return iterator(*this, BOX, box); }

	friend std::ostream& operator<<(std::ostream& os, SudokuGrid& grid);
	friend std::istream& operator>>(std::istream& is, SudokuGrid& grid);

	friend std::ostream& operator<<(std::ostream& os, Group& group);

	void debug_print();
};
