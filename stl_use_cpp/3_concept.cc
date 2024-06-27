#include <bits/stdc++.h>
using namespace std;

template<class T>
concept is_bool_ = is_same_v<T, bool>;

template<class T>
concept is_int_ = is_same_v<T, int>;

template<class T>
requires is_bool_<T>
constexpr T and_(T a, T b)
{
    return a && b;
}

template<class T>
constexpr T and_(T a, T b) requires is_int_<T>
{
    return a & b;
}

int main()
{
    cout << and_(true, false) << "\n";
    cout << bitset<sizeof(int)>(and_(0b11011, 0b00011)) << "\n";
}