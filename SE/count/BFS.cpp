// BFS.cpp
//
#include "BFS.hpp"

#include <stdexcept>

// Need one vitual function hear to kick the vtable into existence.
// Sucks, but. Yeah, it sucks.
ABFS::~ABFS()
{
    // No dynamic data.
}

// This one is static, so it does not do the trick.
// Sucks, but alternative is dynamically-sized BFS thingies (and another layer of indirectin).
ABFS*
ABFS::create(size_t rank, size_t stage)
{
    switch(rank)
    {
      default:
        throw invalid_argument("Unsupported rank.");
      case 3:
        switch(stage)
        {
          default:
            throw invalid_argument("Unsupported rank 3 stage.");
          case 2: return new BFS<3,2>();
          case 1: return new BFS<3,1>();
          case 0: return new BFS<3,0>();
        }
    }
}
