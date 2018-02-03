// test/Butterfly.cpp
//
#include "../Butterfly.hpp"

#include <iostream>

using namespace std;

int main()
{
    Butterfly<3> b;
    b = 6;
    for (size_t i = 0; i < b.size(); ++i)
    {
        cout << i << '\t' << b.idx(i) << endl;
    }
    cout << "Done." << endl;      
}
