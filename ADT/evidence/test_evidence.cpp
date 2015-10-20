#include "evidence.hpp"

#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	Evidence a = Evidence::LAZINESS_TRUE;
	Evidence b = Evidence::LAZINESS_FALSE;
	Evidence c = a & b; // == LAZINESS_UNKNOWN
	Evidence d = a | b; // == LAZINESS_INCONSISTENT

	assert(a.isLazy());
	assert(a.maybeLazy());
	assert(! a.isNotLazy());
	assert(! a.maybeNotLazy());

	assert(! b.isLazy());
	assert(! b.maybeLazy());
	assert(b.isNotLazy());
	assert(b.maybeNotLazy());

	assert(! c.isLazy());
	assert(c.maybeLazy());
	assert(! c.isNotLazy());
	assert(c.maybeNotLazy());

	assert(! d.maybeLazy());
	assert(! d.maybeNotLazy());
	assert(! d.isLazy());
	assert(! d.isNotLazy());

	assert(a.consistentLaziness());
	assert(b.consistentLaziness());
	assert(c.consistentLaziness());
	assert(d.inconsistentLaziness());

	return 0;
}

