#include "../../container/mapF.h"
#include "../../container/multimapF.h"
#include <iostream>
using namespace std;

template <class _O, class _T> _O &operator<<(_O &__cout, _T &__c)
{
    __cout << "{";
    for (auto it : __c) {
        __cout << " " << __c[it.first];
    }
    __cout << " }";

    return __cout;
}

void map_test()
{
    iop::map<int, int> mf;
    iop::pair<const int, int> z(20, 10);
    iop::pair<const int, int> z2(23, 14);
    iop::pair<const int, int> z3(10, 13);
    mf.insert(z);
    mf.insert(z2);
    mf.insert(z3);

    cout << mf << "\n";

    iop::map<string, int> sf;
    iop::map<string, int>::value_type s1 = {"a", 10};
    iop::map<string, int>::value_type s2 = {"b", 20};
    iop::map<string, int>::value_type s3 = {"c", 10};
    sf.insert(s1);
    sf.insert(s2);
    sf.insert(s3);

    cout << sf << "\n";
}

void multimap_test()
{
    iop::multimap<int, int> mf;
    iop::pair<const int, int> z(20, 10);
    iop::pair<const int, int> z2(23, 14);
    iop::pair<const int, int> z3(10, 13);
    mf.insert(z);
    mf.insert(z2);
    mf.insert(z3);

    cout << mf << "\n";

    // for iop::string is wrong
    iop::multimap<string, int> sf;
    iop::multimap<string, int>::value_type s1 = {"a", 10};
    iop::multimap<string, int>::value_type s2 = {"b", 20};
    iop::multimap<string, int>::value_type s3 = {"c", 10};
    sf.insert(s1);
    sf.insert(s2);
    sf.insert(s3);

    cout << sf << "\n";
}

int main()
{
    map_test();
    multimap_test();

    return 0;
}