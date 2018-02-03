#ifdef BFS_hpp
#error Multiple inclusion.
#else
#define BFS_hpp

#include "Butterfly.hpp"

// Abstract butterfly stage.
class ABFS
{
  public: // types
    typedef size_t cidx; // The index of a control element.
    typedef size_t index; // The index of an intput or output.
    typedef unsigned program; // An integer whose bits are interpreted as the
                              // configuration of the bit vector. 
  public: // static methods
    static ABFS* create(size_t rank, size_t stage);

  public: // abstract methods
    virtual ~ABFS();
    ABFS() {}
    ABFS(const ABFS&) = delete;
    virtual ABFS* clone() const = 0;

    virtual void operator=(program p) = 0;
    virtual void to_1(cidx b) = 0;
    virtual void to_0(cidx b) = 0;
//    virtual reference operator[](cidx b) = 0;
    virtual bool operator[](cidx b) const = 0;
    virtual index idx(index i) const = 0;
    virtual index val(index i) const = 0;
    virtual program control() const = 0;
    
  public:
    void operator=(const ABFS& other) { operator=(other.control()); }
};

// Butterfly Stage
// A butterfly stage of rank N always has 2**N inputs and 2**(N-1) control bits.
// Stage 0 has a span of 2**(N-1), i.e. consists of a single Butterfly of rank N.
// Stage 1 has a span of 2**(N-2), consisting of two Butterflies of rank N-1.
// Stage N-1 consists of 2**(N-2) Butterflies of rank 1.
template <size_t rank, size_t stage> class BFS : public ABFS
{
  public: // static constants
    static const size_t SIZE = 1 << rank;      // The number of inputs/outputs
    static const size_t CAP = SIZE >> 1;       // The number of control elements.
    static const size_t BCOUNT = 1 << stage;   // The number of butterflies in this stage.
    static const size_t BSIZE = SIZE >> stage; // The size of a butterfly in this stage.
    static const size_t BCAP = CAP >> stage;   // The capacity of a butterfly in this stage.
    
  public: // static methods
    static size_t size() { return SIZE; }
    static size_t cap() { return CAP; }
    static size_t bcount() { return BCOUNT; }
    static size_t bsize() { return BSIZE; }
    static size_t bcap() { return BCAP; }

  public: // methods
    virtual ~BFS() {} // No dynamic data.
    BFS() {} // Units default-initialized to all zeroes.
    BFS(program p) { init(p); }
    BFS(const BFS& other) { init(other.control()); }
    virtual ABFS* clone() const { return new BFS(*this); }
    void operator=(BFS<rank,stage>&& other) { std::swap(_bfly, other._bfly); }
    void operator=(const BFS<rank,stage>& other) { init(other.control()); }
    virtual void operator=(program p) { init(p); }

    virtual void to_1(cidx b) { _bfly[b / BCAP].to_1(b % BCAP); }
    virtual void to_0(cidx b) { _bfly[b / BCAP].to_0(b % BCAP); }
    typename Butterfly<rank-stage>::bitref
        operator[](cidx b) { return _bfly[b / BCAP][b % BCAP]; }
    virtual bool operator[](cidx b) const { return _bfly[b / BCAP][b % BCAP]; }
    // The input index corresponding to a given output index, for the current
    // bits setting.
    virtual index idx(index i) const;
    // The output index corresponding to a given input index is the same since
    // the transform is symmetrical.
    virtual index val(index i) const { return idx(i); }
    // The control bits, read out as a ulong.
    virtual program control() const;
    
  protected:
    void init(program p);
    
  private:
    Butterfly<rank-stage> _bfly[1<<stage]; 
};

template <size_t rank, size_t stage>
void 
BFS<rank,stage>::init(program p)
{
    for (size_t u = 0; u < BCOUNT; ++u)
    {
        _bfly[u] = p & ~(~0 << BCAP);
        p >>= BCAP;
    }
}

template <size_t rank, size_t stage>
ABFS::index 
BFS<rank,stage>::idx(index i) const
{
    size_t unit = i / BSIZE; // Which butterfly
    size_t input = i % BSIZE; // Which unit
    return unit * BSIZE + _bfly[unit].idx(input);
}

template <size_t rank, size_t stage>
ABFS::program
BFS<rank,stage>::control() const
{
    program result = 0;
    for (size_t u = BCOUNT; u--; )
    {
        result <<= BCAP;
        result |= _bfly[u].control();
    }
    return result;
}

#endif
