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

void SudokuGrid::map_row(unsigned row, Set::set_op op)
{
	for (row_iterator i = this->row_begin(row);
		 i.more(); ++i)
		op(*i);
}

void SudokuGrid::map_column(unsigned column, Set::set_op op)
{
	for (column_iterator i = this->column_begin(column);
		 i.more(); ++i)
		op(*i);
}

void SudokuGrid::map_box(unsigned box, Set::set_op op)
{
	for (box_iterator i = this->box_begin(box);
		 i.more(); ++i)
		op(*i);
}

static char setToChar(Set set)
{
	if (set.count() == 0)
		return ' ';
	if (set.count() > 1)
		return '.';
	for (unsigned i = 0; i < 10; ++i)
		if (set.test(i))
			return '0' + i;
}

std::ostream& operator<<(std::ostream& os, SudokuGrid& grid)
{
	for (unsigned row = 1; row < 10; ++row)
	{
		for (SudokuGrid::row_iterator i = grid.row_begin(row);
			 i.more(); ++i)
			os << ' ' << setToChar(*i);
		os << std::endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, SudokuGrid& grid)
{
	for (unsigned row = 1; row < 10; ++row)
	{
		for (SudokuGrid::row_iterator i = grid.row_begin(row);
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
