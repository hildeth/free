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
		change  = constrain_all(Group::ROW);
		change |= constrain_all(Group::COLUMN);
		change |= constrain_all(Group::BOX);
		_grid.debug_print();
	}
	while (change);
}

bool THHSolver::constrain_all(Group group)
{
	bool change = false;
	for (unsigned index = 1; index < 10; ++index)
		change |= constrain_one(group, index);
	return change;
}

// The algorithm for propagating the current constraints across a group (row,
// column or box) is to
// repeatedly find and apply closures until every digit is accounted for.
// All closures are mutually exclusive, so once a closure has been applied, we
// don't have to look at it again.  Thus if any bit from a position we are
// examining intersects with the "seen" set, we can move on.
//
// The closures across a group should exactly account for all the members of
// that group.  However, if there is a contradiction, that means that the
// available choices at some location in the grid will number zero.
bool THHSolver::constrain_one(Group group, unsigned index)
{
	bool changed = false;
    Set seen; // Initialized to all false.
	while (seen.count() < 9)
	{
		// Returns the available choices among the members of the closure and a
		// bitset representing the members of the closure.
		Set_pair a_m = find_closure(group, index, seen);
		Set& members = a_m.second;
		if (members.count() == 0)
			// If the closure contains zero members, it means we could not find
			// a closure in this group.
			// That means that the puzzle contains a contradiction.
			break;
		Set& available = a_m.first;
		bool change = update(group, index, available, members);
		if (change)
		{
			std::cout << "Found " << group << ' ' << index << " closure "
					  << available << " with members " << members << std::endl;
			changed = true;
		}
		seen |= members;
	}
	return changed;
}

// Find a closure in this index that we have not previously seen.
// The algorithm for finding a closure is very simple.
// We start by looking for closures of size 1
// If no closures of that size exist, we bump the size to 2 and so on.
// Each time a closure is applied, the size is reset to 1.
// When looking for a closure of a given size, we simply or a viable candidate
// with the current closure.  If the size of the closure is less than the limit,
// we proceed.  Otherwise, we reject that candidate and move on.
THHSolver::Set_pair
THHSolver::find_closure(Group group, unsigned index, Set seen)
{
	for (unsigned size = 1; size < 10; ++size)
	{
		// Enumerate permutations at this size.
		for (Set perm = Set::perm_begin(size); perm.perm_more(); perm.perm_next())
		{
			// Reject permutations that intersect with closures already seen.
			if ((perm & seen).count() > 0)
				continue;

			Set avail = set_union(group, index, perm);

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
THHSolver::set_union(Group group, unsigned index, Set perm)
{
	// Represents the digits that are shared among members of the candidate closure.
	Set available; 
	for (unsigned j = 1; j < 10; ++j)
		if (perm.test(j))
			available |= _grid.at(group, index, j);
	return available;
}

// When we find a closure, that means that none of the members of the same
// exclusion class can contain the members of the closure.
// So, for each element not in the closure, we intersect that element with the
// negation of the closure's avaliable set.
bool THHSolver::update(Group group, unsigned index, Set available, Set members)
{
	// Get the available set, inverted.
	Set exclusion = available; exclusion.flip();

	bool change = false;
	for (unsigned j = 1; j < 10; ++j)
	{
		// Don't modify members of the closure.
		if (members[j])
			continue;
		Set oldset = _grid.at(group, index, j);
		Set newset = oldset & exclusion;
		_grid.at(group, index, j) = newset;
		change |= (newset != oldset);
	}
	return change;
}

