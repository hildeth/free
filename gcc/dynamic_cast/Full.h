// Full.h  -*- C++ -*-
// A full class hierarchy.
// Binary tree with 4 levels (15 classes total)

#ifdef FULL_H
#error Multiple inclusion
#else
#define FULL_H

#include "Base.h"

class Full : public Base
{
	static Tag _tag;
  public:
	static Tag tag() { return _tag; }

	~Full() {}
	Full() {}
	virtual bool isA(const Tag& tag) { return _tag == tag; }
	virtual bool derivesFromA(const Tag& tag) { return isA(tag); }
};

#define gen_full_class(Child, Parent) \
class Child : public Parent		  \
{								  \
	static Tag _tag; 	\
  public: \
	static Tag tag() { return _tag; }	  \
									  \
	~Child() {}													 \
	Child() {}													 \
	virtual bool isA(const Tag& tag) { return _tag == tag; }	 \
	virtual bool derivesFromA(const Tag& tag)					 \
	{ return isA(tag) || Parent::derivesFromA(tag); }			 \
};

gen_full_class(FullL, Full)
gen_full_class(FullR, Full)
gen_full_class(FullLL, FullL)
gen_full_class(FullLR, FullL)
gen_full_class(FullRL, FullR)
gen_full_class(FullRR, FullR)
gen_full_class(FullLLL, FullLL)
gen_full_class(FullLLR, FullLL)
gen_full_class(FullLRL, FullLR)
gen_full_class(FullLRR, FullLR)
gen_full_class(FullRLL, FullRL)
gen_full_class(FullRLR, FullRL)
gen_full_class(FullRRL, FullRR)
gen_full_class(FullRRR, FullRR)

#endif
