// evidence.hpp
//
// Copyright (C) 2015 Thomas H. Hildebrandt  All rights reserved.
//


//////////////////////////////////////////////////////////////////////////
// The basic idea is to encode evidence using two bits.
// Each bit represents evidence in favor or against a binary value.
// Both bits being false indicates that there is no information; both bits being
// true indicate that there is conflicting evidence.
//
// A conflict does not mean that the data structure itself is in an invalid
// state, but perhaps that conflicting evidence has been gathered from different
// sources.  For example, from the code:
//  if (p) {
//    if (!p) {
//      /* dead code */
//    }
//  }
// we gather from the first 'if' that p must not be null, and from the second
// 'if' that p must be null.  Both bits being set in the interior of the nested
// clause means that no value of p is consistent with the evidence we have
// gathered about the state of its null-ness.  That code is therefore
// unreachable.
//
class Evidence
{
  public:
    enum Evidence_e
	{
		// We can assign these four values within each two-bit cluster
		UNKNOWN = 0,
		KNOWN_TRUE = 1 << 0,
		KNOWN_FALSE = 1 << 1,
		BOTH = KNOWN_TRUE | KNOWN_FALSE,
		INCONSISTENT = BOTH,

		// Base bits for bit-pairs in the enumeration.
		// These go up by twos.
		NULLITY_BASE = 1 << 0, // Bits to represent nullity.
		SILLINESS_BASE = 1 << 2, // Bits to represent silliness, etc.

		// All zeroes means we have not obtained any information yet.
		NO_EVIDENCE = 0,

		NULLITY_UNKNOWN = NULLITY_BASE * UNKNOWN,
		NULLITY_TRUE = NULLITY_BASE * KNOWN_TRUE,
		NULLITY_FALSE = NULLITY_BASE * KNOWN_FALSE,
		NULLITY_BOTH = NULLITY_TRUE | NULLITY_FALSE,
		NULLITY_INCONSISTENT = NULLITY_BOTH,

		SILLINESS_UNKNOWNN = SILLINESS_BASE * UNKNOWN,
		SILLINESS_TRUE = SILLINESS_BASE * KNOWN_TRUE,
		SILLINESS_FALSE = SILLINESS_BASE * KNOWN_FALSE,
		SILLINESS_BOTH = SILLINESS_TRUE | SILLINESS_FALSE,
		SILLINESS_INCONSISTENT = SILLINESS_BOTH,

		// and so on.
	};

  private:
	Evidence_e ev;

  public:
	~Evidence() {}
	Evidence() : ev(NO_EVIDENCE) {}
	Evidence(Evidence_e e) : ev(e) {}
    Evidence(const Evidence& rhs) : ev(rhs.ev) {}
    void operator=(const Evidence rhs) { ev = rhs.ev; }

	bool isNull() const
	{ return ((unsigned) ev & NULLITY_BOTH) == NULLITY_TRUE; }
	bool isNotNull() const
	{ return ((unsigned) ev & NULLITY_BOTH) == NULLITY_FALSE; }
	// If it's not proven false, it might be true.
	bool maybeNull() const
	{ return ! ((unsigned) ev & NULLITY_FALSE); }
	// If it's not proven true, it might be false.
	bool maybeNotNull() const
	{ return ! ((unsigned) ev & NULLITY_TRUE); }
	bool unknownNullity() const
	{ return ((unsigned) ev & NULLITY_BOTH) == NULLITY_UNKNOWN; }
	bool inconsistentNullity() const
	{ return ((unsigned) ev & NULLITY_BOTH) == NULLITY_BOTH; }
	bool consistentNullity() const { return ! inconsistentNullity(); }


	// Set union -- a credulous combination of evidence.
	// (Evidence from either source is accepted.)
	void operator|=(const Evidence rhs)
	{ ev = (Evidence_e)((unsigned) ev | (unsigned) rhs.ev); }

	// Set intersection -- a skeptical combination of evidence.
	// (Both sources must agree, or evidence is lost.)
	void operator&=(const Evidence rhs)
	{ ev = (Evidence_e)((unsigned) ev & (unsigned) rhs.ev); }

};

Evidence operator|(Evidence a, Evidence b)
{
	a |= b;
	return a;
}

Evidence operator&(Evidence a, Evidence b)
{
    a &= b;
	return a;
}

