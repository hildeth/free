// Set.cpp -*- C++ -*-
//

#include "Set.h"

// Starts out a permutation by setting the initial "size" bits to true.
Set
Set::perm_begin(unsigned size)
{
	Set result;
	for (unsigned i = 1; i <= size; ++i)
		result.set(i);
	return result;
}

// Move to the next bit-permutation.
// The algorithm is:
//  0. Start at the highest-numbered bit position.
//  1. Move down to find a nonzero bit.
//  2. Erase that bit and increment bits-to-set.
//  3. If there is not enough space to write all bits, go to Step 1.
//  4. Move up one bit.
//  5. Write one bit and decrement bits-to-set.
//  6. If bits-to-set is nonzero, go to Step 4.
//
// Note that after the last valid permutation, the algorithm will leave all bits
// zero, making it easy to test for a termination condition in the caller.
void
Set::perm_next()
{
	unsigned bits = 0;	// The number of bits yet to be written out.
    unsigned pos = 10;
	// Moving left.
	do
	{
		// Find a true bit.
		do
		{
			if (pos == 1)
				// No more bits, so done.
				return;
		}
		while (! test(--pos));

		// Reset the bit and increment bits-to-set.
		reset(pos); ++bits;
	}
	// Repeat until there is enough room to write all pending bits.
	while (pos + bits == 10);

	// Now write out all the pending bits in consecutive locations.
	while (bits > 0)
	{
		set(++pos); --bits;
	}
}

// Return a single character to represent the contents of a set:
// <empty> --> ' '
// <one bit set> --> A single digit having the index of the occupied bit position.
// <more bits set> --> '.'
Set::operator char()
{
	if (set.count() == 0)
		return ' ';
	if (set.count() > 1)
		return '.';
	for (unsigned i = 0; i < 10; ++i)
		if (set.test(i))
			return '0' + i;
}

std::ostream& operator<<(std::ostream& os, Set set)
{
	for (unsigned i = 1; i < 10; ++i)
	{
		if (set.test(i))
			os << (char)('0' + i);
		else
			os << '.';
	}
	return os;
}
