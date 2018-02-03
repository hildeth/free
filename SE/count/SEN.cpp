// SEN.cpp
//
#include "DFN.hpp"

#include <map>
#include <iostream>

using namespace std;

const int RANK = 3;

static void
add_count(DFN<RANK>& net, map<unsigned,size_t>& counts)
{
    // Create a signature for the permutation exiting the net
    // using RANK bits per symbol.
    unsigned sig = 0;
    for (size_t i = DFN<RANK>::size(); i--; )
    {
        sig <<= RANK;
        sig += net.idx(i);
    }
    ++counts[sig];
}
        
static void
do_count(DFN<RANK>& net, map<unsigned,size_t>& counts, unsigned stage)
{
    if (stage == 2 * RANK) add_count(net, counts);
    else 
    {
        for (size_t p = 0; p < (1<<DFN<RANK>::cap()); ++p)
        {
            // WLOG: A given configuration is symmetrical with its reflection
            // about the plane bisecting 0-1 in that dimension. Thus, in the
            // first 3 stages, we can fix one of the configurations.
            // We do this by skipping the configurations whose first bit is 1.
            if (stage < RANK && p >= (1<<(DFN<RANK>::cap()-1))) continue;

            net.set(stage, p);
            do_count(net, counts, stage+1);
        }
    }
}

int main()
{
    DFN<RANK> net;
    // Counts the number of ways a given permutation can be created.
    // key = permutation, value = count
    map<unsigned,size_t> counts;
    do_count(net, counts, 1);
    cout << "Permutations:\t" << counts.size() << endl;

    size_t tota = 0;
    // A histogram of counts.
    map<size_t,size_t> hist;
    for (auto& e : counts)
    {
        ++hist[e.second];
        tota += e.second;
    }
    cout << "Configurations:\t" << tota << endl;

    size_t totb = 0;
    for (auto& h : hist)
    {
        cout << h.first << '\t' << h.second << endl;
        totb += h.first * h.second;
    }
    cout << "total:" << '\t' << totb << endl;
    
    return 0;
}
