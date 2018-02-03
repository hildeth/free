// FN.hpp
//
// An F network of the specified rank.
//  consists of one Butterfly of rank N whose outputs are connected to two
//  Butterflies of rank N-1, down to rank 1.

#include "BFS.hpp"

template <size_t rank> class FN
{
  public: // types
    typedef size_t cidx; // The index of a control element.
    typedef size_t index; // The index of an intput or output.
    typedef unsigned program; // An integer whose bits are interpreted as the
                              // configuration of the bit vector. 
  public: // static constants
    static const size_t SIZE = 1 << rank;      // The number of inputs/outputs
    static const size_t CAP = SIZE >> 1;       // The number of control elements.

  public: // static methods    
    // The number of inputs/outputs.
    static size_t size() { return SIZE; }
    static size_t cap() { return CAP; }

  public: // methods
    ~FN();
    FN();
    FN(const FN& other);
    void operator=(const FN& other);
    
    void set(size_t stage, program p);
    program get(size_t stage) const;

    // The input from which the output at index o is selected.
    index idx(index o) const;
    // The output connected to the ith input.
    index ridx(index i) const;
    
  private:
    ABFS* _stage[rank];
};

template <size_t rank>
FN<rank>::~FN()
{
    for (size_t s = 0; s < rank; ++s)
    {
        delete _stage[s]; _stage[s] = 0;
    }
}

template <size_t rank>
FN<rank>::FN()
{
    for (size_t s = 0; s < rank; ++s)
        _stage[s] = ABFS::create(rank, s);
}

template <size_t rank>
FN<rank>::FN(const FN& other)
{
    for (size_t s = 0; s < rank; ++s)
        _stage[s] = other._stage[s].clone();
}

template <size_t rank>
void
FN<rank>::operator=(const FN& other)
{
    for (size_t s = 0; s < rank; ++s)
        *_stage[s] = other.get(s);
}

template <size_t rank>
void
FN<rank>::set(size_t s, program p)
{
    *_stage[s] = (ABFS::program) p;
}

template <size_t rank>
typename FN<rank>::program
FN<rank>::get(size_t s) const
{
    return _stage[s]->control();
}

template <size_t rank>
typename FN<rank>::index
FN<rank>::idx(index in) const
{
    index result = in;
    for (size_t s = rank; s--; )
        result = _stage[s]->idx(result);
    return result;
}

template <size_t rank>
typename FN<rank>::index
FN<rank>::ridx(index in) const
{
    index result = in;
    for (size_t s = 0; s < rank; ++s)
        result = _stage[s]->ridx(result);
    return result;
}

