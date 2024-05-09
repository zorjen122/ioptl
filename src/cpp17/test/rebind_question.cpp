#include <iostream>
#include "../allocator_F.h"
#include "../allocator_traits_F.h"
#include "../type_traits.h"
using namespace std;

template <class _Tp,
          class = typename mpls::enable_if_t<mpls::is_same<int, _Tp>::value>>
struct nothing
{};


template<class _Ty>
struct allocs
{
    template<class T>
    struct rebind
    {
        using other = allocs<T>;
    };

    void print();
};

template<>
struct allocs<int>
{
    template<class T>
    struct rebind
    {
        using other = allocs<T>;
    };

    void print()
    {
        cout << "this is int\n";
    }
};


template<>
struct allocs<long>
{
    template<class T>
    struct rebind
    {
        using other = allocs<T>;
    };

    void print()
    {
        cout << "this is long\n";
    }
};

int main()
{
    using type = allocs<int>;
    // type::print();
    using p = typename type::template rebind<long>::other;
    p e;
    e.print();
           
    return 0;
}