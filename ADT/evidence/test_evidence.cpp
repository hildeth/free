#include "evidence.hpp"

#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	Evidence a = Evidence::NULLITY_TRUE;
	Evidence b = Evidence::NULLITY_FALSE;
	Evidence c = a & b; // == NULLITY_UNKNOWN
	Evidence d = a | b; // == NULLITY_INCONSISTENT

	assert(a.isNull());
	assert(a.maybeNull());
	assert(! a.isNotNull());
	assert(! a.maybeNotNull());

	assert(! b.isNull());
	assert(! b.maybeNull());
	assert(b.isNotNull());
	assert(b.maybeNotNull());

	assert(! c.isNull());
	assert(c.maybeNull());
	assert(! c.isNotNull());
	assert(c.maybeNotNull());

	assert(! d.maybeNull());
	assert(! d.maybeNotNull());
	assert(! d.isNull());
	assert(! d.isNotNull());

	assert(a.consistentNullity());
	assert(b.consistentNullity());
	assert(c.consistentNullity());
	assert(d.inconsistentNullity());

	return 0;
}

