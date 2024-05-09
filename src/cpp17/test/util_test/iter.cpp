#include "../../allocator_F.h"
#include "../../iterator_F.h"
#include "../../iterator_base_F.h"
#include <iostream>

using namespace iop;
void foo1()
{
    using std::cout;
    allocator<int> allc;
    iter::trivial_iterator<int> iter = allc.allocate(10);
    iter::trivial_iterator<int> end = iter + 10;

    for (int i = 0; i < 10; ++i)
        *((iter + i).operator->()) = i;

    for (int i = 0; i < 10; ++i)
        cout << *(iter + i) << " ";

    cout << "\n";
    auto it = iter + 10;
    for (int _ = 0; _ < 10; ++_)
        cout << *(--it) << " ";
    cout << "\n";

    cout << "end - iter :  " << end - iter << "\n";

    int v = 10;
    iter::trivial_iterator<int> p = &v;
    cout << *p << "\n";

    // TODO destroy/deallocate to trivial_iterator.
    
}

int main() { foo1(); }