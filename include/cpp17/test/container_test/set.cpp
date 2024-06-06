#include "../../container/multisetF.h"
#include "../../container/setF.h"
#include "../test_.h"
#include <iostream>
using namespace std;

void set_test()
{
    iop::set<int> sf = {0, 1, 2, 3, 4};

    for (int i = 0; i < 10; ++i) {
        sf.insert(i);
    }

    cout << sf << "\n";
}

void multiset_test()
{
    iop::multiset<int> sf = {0, 1, 2, 3, 4};

    for (int i = 0; i < 10; ++i) {
        sf.insert(i);
    }

    cout << sf << "\n";
}

int main()
{
    cout << "set_test: "
         << "\n";
    set_test();

    cout << "multiset_test: "
         << "\n";
    multiset_test();
    return 0;
}