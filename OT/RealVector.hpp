#ifdef REALVECTOR_HPP
#error Multiple inclusion
#else
#define REALVECTOR_HPP

#include <vector>

using namespace std;

class RealVector : public vector<double>
{
  public:
    ~RealVector() {}
    RealVector() : vector<double>() {}
    RealVector(const RealVector& rhs) : vector<double>(rhs) {}
    RealVector(RealVector&& rhs) : vector<double>(rhs) {}
    // I don't believe in chained assignment.
    void operator=(const RealVector& rhs)
    { this->vector<double>::operator=(rhs); }
    void operator=(RealVector&& rhs)
    { this->vector<double>::operator=(rhs); }

    // Destructive binary addition operator.
    void operator+=(const RealVector& rhs);
};

// Non-destructive binary addition operators
RealVector* operator+(const RealVector& a, const RealVector& b);
RealVector* operator+(RealVector*&& a, const RealVector& b);

// A twist -- This declaration:
//   RealVector* operator+(RealVector*& a, const RealVector& b);
// used to be legal, but the type modifier *&& is now required to bind to an
// expiring pointer temporary.
// The signature still expresses what I intend: that you bind to and consume a
// dynamically-allocated object.  In fact, using the && type modifier for this
// purpose is pretty cool.

#endif
