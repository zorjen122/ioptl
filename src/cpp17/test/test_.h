#pragma once

#define _IOP_TEST_
#ifdef _IOP_TEST_

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

#endif