#include "SudokuGrid.h"

void SudokuGrid::init_copy(const SudokuGrid& rhs)
{
	for (unsigned row = 1; row < 10; ++row)
		for (unsigned col = 1; col < 10; ++col)
			_grid[row][col] = rhs._grid[row][col];
}

// Access an element using box coordinates.
// Boxes are numbered in lexical order T-B L-R, thus:
//  1 2 3
//  4 5 6
//  7 8 9
// and the elements within each box are numbered in the same way.
Set&
SudokuGrid::at_box(unsigned box, unsigned index)
{
	unsigned row = 1 + ((box - 1) / 3) * 3
		+ (index - 1) / 3;
	unsigned column = 1 + ((box - 1) % 3) * 3
		+ (index - 1) % 3;
	return _grid[row][column];
}

// Returns the set stored in a location in the grid, selected using one of three
// addressing schemes: ROW, COLUMN, or BOX.
// ROW: Row-major ordering -- (row, column)
// COLUMN: Column-major ordering -- (column, row)
// BOX: Box and sub-box, both enumerated top-to-bottom, left to right.  See at_box() for
// details.
Set&
SudokuGrid::at(Group group, unsigned group_index, unsigned index)
{
	switch (group)
	{
	  case ROW:		return at(group_index, index);
	  case COLUMN:	return at(index, group_index);
	  case BOX:		return at_box(group_index, index);
	}
}

std::ostream& operator<<(std::ostream& os, SudokuGrid& grid)
{
	for (unsigned row = 1; row < 10; ++row)
	{
		for (SudokuGrid::iterator i = grid.row_begin(row);
			 i.more(); ++i)
			os << ' ' << (char)(*i);
		os << std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, SudokuGrid& grid)
{
	for (unsigned row = 1; row < 10; ++row)
	{
		for (SudokuGrid::iterator i = grid.row_begin(row);
			 i.more(); ++i)
		{
			char c;
			bool saw_one = false;
			while (! saw_one)
			{
				is.get(c);
				if (c == '.')
				{
					(*i).set();		// Set all bits.
					(*i).reset(0);	// Reset the zero bit.
					break;
				}
				if (c > '0' && c <= '9')
				{
					(*i).set(c - '0');
					break;
				}
			}
		}
	}
	return is;
}

std::ostream& operator<<(std::ostream& os, SudokuGrid::Group& group)
{
	switch(group)
	{
	  case SudokuGrid::ROW:		os << "row";	break;
	  case SudokuGrid::COLUMN:	os << "column"; break;
	  case SudokuGrid::BOX:		os << "box";	break;
	}
	return os;
}

void SudokuGrid::debug_print()
{
	for (unsigned row = 1; row < 10; ++row)
	{
		for (unsigned col = 1; col < 10; ++col)
		{
			std::cout << ' ';
			std::cout << _grid[row][col];
			if (col % 3 == 0)
				std::cout << "  ";
		}
		std::cout << std::endl;
		if (row % 3 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;
}
