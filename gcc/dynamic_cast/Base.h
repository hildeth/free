// Base.h -*- C++ -*-
//
// This is an abstract base class that defines the interface 
// that all test classes must implement.
//

#ifdef BASE_H
#error Multiple inclusion
#else
#define BASE_H

#include "Tag.h"

class Base
{
	Base(const Base&); // Not implemented.  No copying Base objects.
	void operator=(const Base&); // Not implemented. No assigning Base objects.
  public:
	virtual ~Base() {}
	Base() {}
	virtual bool isA(const Tag& tag) = 0;
	virtual bool derivesFromA(const Tag& tag) = 0;
};

#endif
