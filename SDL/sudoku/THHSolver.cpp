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
		// Enumerate permutations at this size.
		for (Set perm = Set::perm_begin(size); perm.perm_more(); perm.perm_next())
		{
			// Reject permutations that intersect with closures already seen.
			if ((perm & seen).count() > 0)
				continue;

			Set avail = row_set_union(row, perm);

			// Does the closure contain the desired bitcount?
			if (avail.count() == size)
				// Found a closure. Return it.
				return Set_pair(avail, perm);

			// Otherwise, keep looking.
		}
	}
	// We failed to find a closure.
	// Return an empty closure to signal this.
	return Set_pair();
}

// Performance note: We could stop this loop early if the bit count exceeds
// size.  At what point does this become advantageous?
Set
THHSolver::row_set_union(unsigned row, Set perm)
{
	// Represents the digits that are shared among members of the candidate closure.
	Set available; 
	for (unsigned j = 1; j < 10; ++j)
		if (perm.test(j))
			available |= _grid.at(row, j);
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
	for (unsigned j = 1; j < 10; ++j)
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
	bool change = false;
    Set seen; // Initialized to all false.
	while (seen.count() < 9)
	{
		// Returns the available choices among the members of the closure and a
		// bitset representing the members of the closure.
		Set_pair a_m = find_column_closure(column, seen);
		Set& available = a_m.first;
		Set& members = a_m.second;
		change |= update_column(column, available, members);
		seen |= members;
	}
	return change;
}

// Find a closure in this column that we have not previously seen.
THHSolver::Set_pair
THHSolver::find_column_closure(unsigned column, Set seen)
{
	for (unsigned size = 1; size < 10; ++size)
	{
		// Enumerate permutations at this size.
		for (Set perm = Set::perm_begin(size); perm.perm_more(); perm.perm_next())
		{
			// Reject permutations that intersect with closures already seen.
			if ((perm & seen).count() > 0)
				continue;

			Set avail = column_set_union(column, perm);

			// Does the closure contain the desired bitcount?
			if (avail.count() == size)
				// Found a closure. Return it.
				return Set_pair(avail, perm);

			// Otherwise, keep looking.
		}
	}
	// We failed to find a closure.
	// Return an empty closure to signal this.
	return Set_pair();
}

Set
THHSolver::column_set_union(unsigned column, Set perm)
{
	// Represents the digits that are shared among members of the candidate closure.
	Set available; 
	for (unsigned i = 1; i < 10; ++i)
		if (perm.test(i))
			available |= _grid.at(i, column);
}

// When we find a closure, that means that none of the members of the same
// exclusion class can contain the members of the closure.
// So, for each element not in the closure, we intersect that element with the
// negation of the closure's avaliable set.
bool THHSolver::update_column(unsigned column, Set available, Set members)
{
	// Get the available set, inverted.
	Set exclusion = available; exclusion.flip();

	bool change = false;
	for (unsigned i = 1; i < 10; ++i)
	{
		// Don't modify members of the closure.
		if (members[i])
			continue;
		Set oldset = _grid.at(i, column);
		Set newset = oldset & exclusion;
		_grid.at(i, column) = newset;
		change |= (newset != oldset);
	}
	return change;
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
	bool change = false;
    Set seen; // Initialized to all false.
	while (seen.count() < 9)
	{
		// Returns the available choices among the members of the closure and a
		// bitset representing the members of the closure.
		Set_pair a_m = find_box_closure(box, seen);
		Set& available = a_m.first;
		Set& members = a_m.second;
		change |= update_box(box, available, members);
		seen |= members;
	}
	return change;
}

// Find a closure in this box that we have not previously seen.
THHSolver::Set_pair
THHSolver::find_box_closure(unsigned box, Set seen)
{
	for (unsigned size = 1; size < 10; ++size)
	{
		// Enumerate permutations at this size.
		for (Set perm = Set::perm_begin(size); perm.perm_more(); perm.perm_next())
		{
			// Reject permutations that intersect with closures already seen.
			if ((perm & seen).count() > 0)
				continue;

			Set avail = box_set_union(box, perm);

			// Does the closure contain the desired bitcount?
			if (avail.count() == size)
				// Found a closure. Return it.
				return Set_pair(avail, perm);

			// Otherwise, keep looking.
		}
	}
	// We failed to find a closure.
	// Return an empty closure to signal this.
	return Set_pair();
}

Set
THHSolver::box_set_union(unsigned box, Set perm)
{
	// Represents the digits that are shared among members of the candidate closure.
	Set available; 
	for (unsigned j = 1; j < 10; ++j)
		if (perm.test(j))
			available |= _grid.at_box(box, j);
}

// When we find a closure, that means that none of the members of the same
// exclusion class can contain the members of the closure.
// So, for each element not in the closure, we intersect that element with the
// negation of the closure's avaliable set.
bool THHSolver::update_box(unsigned box, Set available, Set members)
{
	// Get the available set, inverted.
	Set exclusion = available; exclusion.flip();

	bool change = false;
	for (unsigned i = 1; i < 10; ++i)
	{
		// Don't modify members of the closure.
		if (members[i])
			continue;
		Set oldset = _grid.at_box(box, i);
		Set newset = oldset & exclusion;
		_grid.at_box(box, i) = newset;
		change |= (newset != oldset);
	}
	return change;
}

