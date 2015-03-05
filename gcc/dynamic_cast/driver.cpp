// Test driver for dynamic_cast and other implementations.
//
// See how long it takes to down-cast a pointer to an object of a base class type to a
// specific derived class.
//
// Results may be very different, depending on how deep the tree is, so we have three
// different test hierarchies:
//  Flat -- Every derived type inherits from the same base type.
//  Full -- A full tree.
//  Tall -- Every class has only one child.

#include <iostream>

#include "Full.h"

// Assume that each iteration lasts about a microsecond and we want to spend 
// at least a second of wall time.  So we need a million iterations
static const size_t count = 10000000;

inline Base* getFull(size_t i)
{
	// Return an object of a subclass of Full about half the time, 
	// and return NULL the rest of the time.
	switch(i % 32)
	{
	  default: return 0;
	  case 1: return new Full();
	  case 2: return new FullL();
	  case 3: return new FullR();
	  case 4: return new FullLL();
	  case 5: return new FullLR();
	  case 6: return new FullRL();
	  case 7: return new FullRR();
	  case 8: return new FullLLL();
	  case 9: return new FullLLR();
	  case 10: return new FullLRL();
	  case 11: return new FullLRR();
	  case 12: return new FullRLL();
	  case 13: return new FullRLR();
	  case 14: return new FullRRL();
	  case 15: return new FullRRR();
	}
}

inline static Full* downcast_using_virtual_call(Base* b)
{
	if (b && b->derivesFromA(Full::tag()))
		return static_cast<Full*>(b);
	else
		return 0;
}

inline static Full* downcast_using_dynamic_cast(Base* b)
{
	return dynamic_cast<Full*>(b);
}

static void test_full()
{
	for (size_t i = 0; i < count; ++i)
	{
		Base* b = getFull(i);
		volatile Full* f;
		f = downcast_using_virtual_call(b);
		f = downcast_using_dynamic_cast(b);
		delete f;
	}
}

int main(int argc, char* argv[])
{
	test_full();

	return 0;
}

