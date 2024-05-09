/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 */

#pragma once

#ifndef _ALGOBASE_IOP_
#define _ALGOBASE_IOP_

#include "cppconfig.h"
#include "utilitys.h"

#include <string.h>

template <class _Ty, _Ty... _args> struct _Args_is_unitvalue
{
    static constexpr _Ty value = _Ty();
    static constexpr size_t N = 0;
};

template <class _Ty, _Ty _val> struct _Args_is_unitvalue<_Ty, _val>
{
    static constexpr _Ty value = _val;
    static constexpr size_t N = 1;
};

template <class _Iter, class _Ty>
IOP_CONSTEXPR_CXX14 void fill(_Iter __first, _Iter __last, const _Ty &value)
{
    for (; __first != __last; ++__first)
        *__first = value;
}

template <class _Iter, class _Size, class _Ty>
IOP_CONSTEXPR_CXX14 void fill_n(_Iter __first, _Size __n, const _Ty &__val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
}

IOP_CONSTEXPR_CXX14 void fill(unsigned char *__first, unsigned char *__last,
                              const unsigned char &__c)
{
    unsigned char __tmp = __c;
    memset(__first, __tmp, __last - __first);
}

IOP_CONSTEXPR_CXX14 void fill(signed char *__first, signed char *__last,
                              const signed char &__c)
{
    signed char __tmp = __c;
    memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

IOP_CONSTEXPR_CXX14 void fill(char *__first, char *__last, const char &__c)
{
    char __tmp = __c;
    memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

template <class _Ty> IOP_CONSTEXPR_CXX14 void swap_ptr(_Ty *_ptra, _Ty *_ptrb)
{
    if (_ptrb != nullptr || _ptrb != nullptr) {
        auto&& _inner = Fiop::move(*_ptra);
        *_ptra = Fiop::move(*_ptrb);
        *_ptrb = _inner;
    }
}

template <class _Ty> _Ty find(_Ty __first, _Ty __last, size_t __v)
{
    _Ty it = __first;
    for (; it != __last; ++it) {
        if (*it == __v)
            break;
    }
    return it;
}

#endif //* _ALGOBASE_IOP_
