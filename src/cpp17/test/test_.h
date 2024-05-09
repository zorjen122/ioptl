#ifndef _IOP_TEST_
#define _IOP_TEST_


#include <chrono>
#include <iostream>
#include "../functionals.h"

template <class _O, class _T>
_O& operator<<(_O& __cout, _T& __c)
{   
    __cout << "{";
    for(auto it = __c.begin(); it != __c.end(); ++it)
    {
        __cout << " " << *it;
    }
    __cout << " }";

    return __cout;
}

void testFunc(iop::function<void()> __f) {
    auto start = std::chrono::high_resolution_clock::now(); // 记录开始时间

    __f();

    auto end = std::chrono::high_resolution_clock::now(); // 记录结束时间
    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count();

    std::cout << duration << " mil\n";
}

#endif