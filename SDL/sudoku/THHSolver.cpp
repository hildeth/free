// THHSolver.cpp
//

#include "THHSolver.h"

//################################################################################{
//# Solving sudoku is just a matter of propagating the constraints introduced by
//# the initial digits until only compatible assignments remain.  After the
//# complete propagation of cosntraints, if any squares (there must be at least
//# two) contain two or more possibilities, then the puzzle has multiple
//# solutions.
//################################################################################}

void THHSolver::solve()
{
	bool change;
	do
	{
		change = constrain_rows();
		change |= constrain_columns();
		change |= constrain_boxes();
		_grid.debug_print();
	}
	while (change);
}

bool THHSolver::constrain_rows()
{
	bool change = false;
	for (unsigned row = 1; row < 10; ++row)
		change |= constrain_row(row);
	return change;
}

// The algorithm for propagating the current constraints across a row is to
// repeatedly find an apply closures until every digit is accounted for.
// All closures are mutually exclusive, so once a closure has been applied, we
// don't have to look at it again.  Thus if any bit from a position we are
// examining intersects with the "seen" set, we can move on.

// The algorithm for finding a closure is very simple.
// We start by looking for closures of size 1
// If no closures of that size exist, we bump the size to 2 and so on.
// Each time a closure is applied, the size is reset to 1.
// When looking for a closure of a given size, we simply or a viable candidate
// with the current closure.  If the size of the closure is less than the limit,
// we proceed.  Otherwise, we reject that candidate and move on.
bool THHSolver::constrain_row(unsigned row)
{
	bool change = false;
    Set seen; // Initialized to all false.
	while (seen.count() < 9)
	{
		// Returns the available choices among the members of the closure and a
		// bitset representing the members of the closure.
		Set_pair a_m = find_row_closure(row, seen);
		Set& available = a_m.first;
		Set& members = a_m.second;
		change |= update_row(row, available, members);
		seen |= members;
	}
	return change;
}

// Find a closure in this row that we have not previously seen.
THHSolver::Set_pair
THHSolver::find_row_closure(unsigned row, Set seen)
{
	for (unsigned size = 1; size < 10; ++size)
	{
		Set_pair result = find_row_closure_size(row, seen, size);
		// Does the closure contain enough members?
		if (result.second.count() == size)
			// Yes, return it.
			return result;
		// Otherwise, keep looking.
	}
	// We failed to find a closure.
	// Return an empty closure to signal this.
	return Set_pair();
}

THHSolver::Set_pair 
THHSolver::find_row_closure_size(unsigned row, Set seen, unsigned size)
{
	// Represents the digits that are shared among members of the closure.
	Set available; 
	// Represents the positions along the row that are included in the closure.
	Set members;

	for (unsigned j = 1; j < 10; ++j)
	{
		// Filter out bit positions that we've already considered
		// That is, we have already used them in a closure to constrain this
		// row.
		if (seen[j])
			continue;

		// Test to see if the available digits at this position in the grid
		// will "fit" into the current closure.
		// To fit, the total true bits in the union of the current closure
		// must fit within the prescribed closure size.
		if ((available | _grid.at(row, j)).count() > size)
			continue;

		available |= _grid.at(row, j);
		members.set(j);

		if (members.count() == size)
			// We found a closure.
			break;
	}

	// If we reach here by exiting the loop, it means we failed to find enough
	// members for a closure of this size.  So to determine success, the caller
	// can test whether members.count() == size.
	return Set_pair(available, members);
}

// When we find a closure, that means that none of the members of the same
// exclusion class can contain the members of the closure.
// So, for each element not in the closure, we intersect that element with the
// negation of the closure's avaliable set.
bool THHSolver::update_row(unsigned row, Set available, Set members)
{
	// Get the available set, inverted.
	Set exclusion = available; exclusion.flip();

	bool change = false;
	for (unsigned j = 0; j < 10; ++j)
	{
		// Don't modify members of the closure.
		if (members[j])
			continue;
		Set oldset = _grid.at(row, j);
		Set newset = oldset & exclusion;
		_grid.at(row, j) = newset;
		change |= (newset != oldset);
	}
	return change;
}

bool THHSolver::constrain_columns()
{
	bool change = false;
	for (unsigned column = 1; column < 10; ++column)
		change |= constrain_column(column);
	return change;
}

bool THHSolver::constrain_column(unsigned column)
{
	return false; // Fail.
}

bool THHSolver::constrain_boxes()
{
	bool change = false;
	for (unsigned box = 1; box < 10; ++box)
		change |= constrain_box(box);
	return change;
}

bool THHSolver::constrain_box(unsigned box)
{
	return false; // Fail.
}
