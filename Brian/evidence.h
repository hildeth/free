#include <bitset>

#include "myenum.hpp"

class Evidence
{
    // Negative and positive evidence.
    std::bitset<(int)MyEnum::Max> pos;
    std::bitset<(int)MyEnum::Max> neg;
    
  public:
    Evidence() {}
    // Initialize the pos and neg bit vectors from the given integers.
    Evidence(int pos, int neg) : pos(pos), neg(neg) {}

    bool isAmbiguous(MyEnum::T which)
    { return isAmbiguous((int)which); }

    bool isAmbiguous(unsigned int which)
    { return !pos[which] && !neg[which]; }

    bool isTrue(MyEnum::T which)
    { return isTrue((int) which); }

    bool isTrue(unsigned int which)
    { return pos[which] && !neg[which]; }

    bool isFalse(MyEnum::T which)
    { return isFalse((int) which); }

    bool isFalse(unsigned int which)
    { return !pos[(int) which] && neg[(int) which]; }

    bool isConficting(MyEnum::T which)
    { return isConflicting((int) which); }

    bool isConficting(unsigned int which)
    { return pos[(int) which] && neg[(int) which]; }

    void addpos(MyEnum::T which)
    { pos.set((int) which); }
    
    void addneg(MyEnum::T which)
    { neg.set((int) which); }

    unsigned int size()
    { return (int) MyEnum::Max;}
};
