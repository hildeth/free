#include "SudokuGrid.h"

void SudokuGrid::init_copy(const SudokuGrid& rhs)
{
	for (unsigned row = 1; row < 10; ++row)
		for (unsigned col = 1; col < 10; ++col)
			_grid[row][col] = rhs._grid[row][col];
}

void SudokuGrid::map_row(unsigned row, set_op op)
{
	for (row_iterator i = this->row_begin(row);
		 i.more(); ++i)
		op(*i);
}

void SudokuGrid::map_column(unsigned column, set_op op)
{
	for (column_iterator i = this->column_begin(column);
		 i.more(); ++i)
		op(*i);
}

void SudokuGrid::map_box(unsigned box, set_op op)
{
	for (box_iterator i = this->box_begin(box);
		 i.more(); ++i)
		op(*i);
}

static char setToChar(SudokuGrid::Set set)
{
	if (set.count() != 1)
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

static void debug_print_set(const SudokuGrid::Set& avail)
{
	for (unsigned i = 1; i < 10; ++i)
	{
		if (avail.test(i))
			std::cout << (char)('0' + i);
		else
			std::cout << '.';
	}
}

void SudokuGrid::debug_print()
{
	for (unsigned row = 1; row < 10; ++row)
	{
		for (SudokuGrid::row_iterator i = row_begin(row);
			 i.more(); ++i)
		{
			std::cout << ' ';
			debug_print_set(*i);
		}
		std::cout << std::endl;
	}
}
