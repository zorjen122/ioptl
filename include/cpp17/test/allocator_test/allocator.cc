#include <iostream>

#include "../../allocatorF.h"
#include "../../traits/allocator_traitsF.h"
using std::cout;

NAMESPACE_BEGIN(iop)

void func()
{

    using type = allocator<int>;
    using alloc_t = typename allocator_traits<type>::template rebind_alloc<char>::other;
    using traits_t = allocator_traits<type>::template rebind_traits<alloc_t>::other;
    alloc_t alloc;

    char* p = alloc.allocate(10);

    for(int i = 0; i < 10; ++i)
        traits_t::construct(p + (i - '0'), 'a' + (i - '0'));
    
    for(int i = 0; i < 10; ++i)
        cout << *(p + (i - '0')) << " ";

// test destory ..., user-define allocator.

    cout << "\n";
}


NAMESPACE_END(iop)

int main()
{
    iop::func();
}