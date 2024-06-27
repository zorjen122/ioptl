#include <bits/stdc++.h>
using namespace std;

struct Int
{
    int x;
    Int(int _x) : x(_x) {}

    auto operator<=>(const Int& rhs) const = default;
    auto operator==(const Int& rhs) const { return (*this <=> rhs) == 0; }
};

int main()
{
    Int a = 10, b = 20;
    assert(a!=b);
    assert(a < b);
}
