// test-BFS/test.cpp
//
#include "BFS.hpp"

#include <iostream>

using namespace std;

template <size_t rank, size_t stage> void test_bfs()
{
    BFS<rank,stage> b(6);
    for (size_t i = 0; i < b.size(); ++i)
    {
        cout << i << '\t' << b.idx(i) << endl;
    }
    cout << "Done." << endl;      
}

int main()
{
    test_bfs<3,0>();
    test_bfs<3,1>();
    test_bfs<3,2>();
    return 0;
}

