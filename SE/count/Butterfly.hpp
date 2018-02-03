#ifdef Butterfly_hpp
#error Multiple inclusion.
#else
#define Butterfly_hpp

#include <bitset>
#include <cinttypes>

#include <iostream>

using namespace std;

// A butterfly network of rank N has 2**N inputs and 2**(N-1) control bits.
template <size_t rank> class Butterfly
{
  public: // static constants
    // The number of inputs/output of this butterfly stage.
    static const size_t SIZE = 1 << rank;
    // The number of bits in its control word.
    static const size_t CAP = SIZE >> 1;

  public: // types
    typedef size_t cidx; // The index of a control element.
    typedef size_t index; // The index of an intput or output.
    typedef unsigned program; // An integer whose bits are interpreted as the
                              // configuration of the bit vector.
    typedef typename bitset<CAP>::reference bitref;

  public: // static methods
    static const size_t size() { return SIZE; }
    static const size_t cap() { return CAP; }

  public: // methods
    ~Butterfly() {} // No dynamic data.
    Butterfly() {} // _bits default-initialized to all zero.
    Butterfly(program p) : _bits(p) {}
    Butterfly(const Butterfly& other) : _bits(other._bits) {}
    Butterfly(Butterfly&& other) { std::move(_bits, other._bits); }
    void operator=(const Butterfly<rank>& other) { _bits = other._bits; }
    void operator=(Butterfly<rank>&& other) { swap(_bits, other._bits); }
    void operator=(program p) { _bits = p; }
    
    void to_1(cidx b) { _bits.set(b); }
    void to_0(cidx b) { _bits.reset(b); }
    typename bitset<CAP>::reference operator[](cidx b) { return _bits[b]; }
    bool operator[](cidx b) const { return _bits[b]; }
    // The input index corresponding to a given output index, for the current
    // bits setting.
    index idx(index i) const 
    { return (i + CAP * _bits[i % CAP]) % SIZE; }
    // The output index corresponding to a given input index is the same since
    // the transform is symmetrical.
    index ridx(index i) const { return idx(i); }
    // The control bits, read out as a ulong.
    program control() const { return _bits.to_ulong(); }
    
  private: // data
    bitset<CAP> _bits;
};

#endif
