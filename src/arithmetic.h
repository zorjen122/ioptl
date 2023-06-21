#pragma once

#define _ARITHMETIC_STL_
#ifdef _ARITHMETIC_STL_

template<class _Ty>
void swap(_Ty& __q, _Ty& __p) {
    const _Ty __inner = __q;
    __q = __p;
    __p = __inner;
}


#endif

