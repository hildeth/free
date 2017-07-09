#include "RealVector.hpp"
#include <stdexcept>

void RealVector::operator+=(const RealVector& rhs)
{
    if (rhs.size() != this->size()) throw domain_error("Sizes must match");
    RealVector::iterator it;
    RealVector::const_iterator it2;
    for (it = begin(), it2 = rhs.begin();
         it != end();
         ++it, ++it2)
        *it += *it2;
}

RealVector* operator+(const RealVector& a, const RealVector& b)
{
    RealVector* result = new RealVector(a); // Verbatim copy.
    *result += b;
    return result;
}

// This one consumes the object passed to it as a pointer.
RealVector* operator+(RealVector*&& a, const RealVector& b)
{
    RealVector* result = a; a = 0; // Consume a.
    *result += b;
    return result;
}

#ifdef UNIT_TEST

#include <iostream>

int main()
{
    RealVector rv1,rv2,rv3;
    rv1.push_back(3.14159);
    rv2.push_back(2.71828);
    rv3.push_back(1.41421);
    RealVector* rv = rv1 + rv2 + rv3;
    for (auto e : *rv)
        std::cout << e << std::endl;
    delete rv; rv=0;
    return 0;
}

// Things to note:
//  - The return value of the first addition (rv1 + rv2) is of type RealVector*
//  - The unnamed temporary binds to the first operand in the second overload of
//    operator+().
//  - The temporary object itself is used to accumulate the result.  So in a
//    very long, chained operation like:
//      auto rv = rv1 + rv2 + rv3 + rv4 + rv5 + rv6 + ... ;
//    there would actually only be *one* temporary object used for all
//    intermediate calculations (and also containing the result).
// and that's also pretty cool.
#endif

