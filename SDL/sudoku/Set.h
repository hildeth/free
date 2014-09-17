// Set.h  -*- C++ -*-
//

#include <bitset>
#include <iostream>

// A Set represents the digits that may be placed in a given box.
class Set : public std::bitset<10>
{
  public:
	typedef void (*set_op)(Set&);
	typedef std::bitset<10> base;
	typedef base& reference;
	typedef base* pointer;

 public:
	~Set() {}
	Set() {}
	Set(base b)		// Promotion constructor.
		: std::bitset<10>(b) {}
	Set(const Set& rhs)	
		: std::bitset<10>((reference) rhs) {}
	void operator=(const Set& rhs)
		{ *(pointer)this = (reference) rhs; }

	// Find bit-set permutations.
	static Set perm_begin(unsigned size);
	bool perm_more() { return count() > 0; }
	void perm_next();

	explicit operator char();

	friend std::ostream& operator<<(std::ostream& os, Set s);
};

