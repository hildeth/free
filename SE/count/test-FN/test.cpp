// test-FN/test.cpp
//
#include "FN.hpp"

#include <iostream>

using namespace std;

static void print_FN(const FN<3>& f);

int main()
{
    FN<3> f;

    print_FN(f);
    f.set(0,6);
    print_FN(f);
    f.set(1,5);
    print_FN(f);
    f.set(2,4);
    print_FN(f);

    return 0;
}

static void print_FN(const FN<3>& f)
{
    for (size_t i = 0; i < f.size(); ++i)
    {
        cout << i << '\t' << f.idx(i) << endl;
    }
    cout << "Done." << endl;
}
