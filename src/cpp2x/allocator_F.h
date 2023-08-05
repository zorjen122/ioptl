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

#define _ALLOC_OPERATOR_NEW_IOP_
#ifdef _ALLOC_OPERATOR_NEW_IOP_

#include "utilitys.h"
#include "cppconfig.h"
#include <type_traits>

namespace iop {

    template<class _Ty>
    class __Default_alloc {
    public:
        using value_type = _Ty;
        using pointer = _Ty*;
        using const_pointer = const _Ty*;
        using void_pointer = void*;
        using const_void_pointer = const void*;

        using size_type = size_t;
        using difference_type = ::std::ptrdiff_t;


        #if IOP_VERSION > 20
        [[nodiscard]] IOP_CONSTEXPR_CXX23
            __allocation_result<_Ty> allocate_at_least(size_type __n) {
            return { allocate(__n), __n };
        }
        #endif

        IOP_STATIC_CONSTEXPR_CXX17 void deallocate(pointer __ptr, const size_type __n) {
            if (__ptr != nullptr) ::operator delete(__ptr);
        }
        IOP_STATIC_CONSTEXPR_CXX17 void deallocate(pointer __ptr) {
            if (__ptr != nullptr) ::operator delete(__ptr);
        }

        IOP_STATIC_CONSTEXPR_CXX17 pointer allocate(size_type __n) {
            return 0 == __n ? nullptr :
                static_cast<pointer>(::operator new(__n * sizeof(value_type)));
        }

        IOP_STATIC_CONSTEXPR_CXX17 pointer allocate(void) {
            return static_cast<pointer>(::operator new(sizeof(value_type)));
        }
    };

    template<class _Ty>
    class __Default_alloc<const _Ty> {
    public:
        using value_type = const _Ty;
        using size_type = size_t;
        using difference_type = ::std::ptrdiff_t;


        IOP_CONSTEXPR __Default_alloc() NOEXCEPT = default;

        template<class _Tp>
        IOP_CONSTEXPR __Default_alloc(const __Default_alloc<_Tp>&) NOEXCEPT {}

        #if IOP_VERSION > 20
        [[nodiscard]] IOP_CONSTEXPR_CXX23
            __allocation_result<const _Ty> allocate_at_least(const size_type __n) {
            return { allocate(__n), __n };
        }
        #endif

        IOP_STATIC_CONSTEXPR_CXX17 void deallocate(const _Ty* __ptr, const size_type __n) {
            if (__ptr != nullptr) ::operator delete(const_cast<_Ty*>(__ptr));
        }
        IOP_STATIC_CONSTEXPR_CXX17 void deallocate(const _Ty* __ptr) {
            if (__ptr != nullptr) ::operator delete(const_cast<_Ty*>(__ptr));
        }


        const _Ty* allocate(size_type __n) {
            return 0 == __n ? nullptr :
                const_cast<_Ty*>(::operator new(__n * sizeof(value_type)));
        }

        const _Ty* allocate(void) {
            return
                const_cast<_Ty*>(::operator new(sizeof(value_type)));
        }

    };      //* namespace iop


    template<class _Ty>
    struct allocator : public __Default_alloc<_Ty> {};

}

#endif      //* _ALLOC_OPERATOR_NEW_IOP_
