/*
 * Copyright (c) 1997    -zorjen122
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */


#pragma once

#define _INITAILIZER_LIST_IOP_DEFINE_
#ifdef _INITAILIZER_LIST_IOP_DEFINE_

#include "../iterator_F.h"
#include "../allocator_traits_F.h"
#include "../allocator_traits_F.h"
#include "../allocator_F.h"


namespace iop {

    template<class _Ty>
    class initializer_list {
    public:
        explicit initializer_list() : _Vlist(nullptr), _len(0) {}
        explicit initializer_list(const _Ty* __src, size_t __s) : _Vlist(__src), _len(__s) {}

    protected:
        const _Ty* _Vlist;
        size_t _len;

    public:

        size_t size() const { return _len; }

        const _Ty* begin() { return _Vlist; }

        const _Ty* end() { return _Vlist + _len; }

        bool empty() { return (begin() == end()); }

        const _Ty* data() { return _Vlist; }

    };


    template<class _T>
    const _T* begin(const initializer_list<_T>& __r) { return __r.begin(); }

    template<class _T>
    const _T* end(const initializer_list<_T>& __r) { return __r.end(); }

};                    // namespace iop

#endif                // _INITAILIZER_LIST_IOP_DEFINE_