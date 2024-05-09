#include <iostream>
#include "../../allocator_F.h"
#include "../../allocator_traits_F.h"
using std::cout;

NAMESPACE_BEGIN(iop)

template<class Alloc>
struct __allocator_traits
{
    template<class T>
    struct rebind
    {
        using other = iop::allocator<T>;
    };

};

void func()
{
    
    using type = allocator<int>;
    // __allocator_traits<type>::template rebind<char>::other alloc;
    iop::allocator_traits<type>::template rebind<char>::other alloc;

    char* v = alloc.allocate(10);

    cout << "sizeof v: " << sizeof(v) << "\n";

    char c = 'a';
    for(int i = 0; i < 10; ++i)
    {
        *(v + i) = c++; 
        cout << *(v + i) << " ";
    }

    cout << "\n";

}


NAMESPACE_END(iop)

int main()
{
    iop::func();
}