/*
 * Copyright (c) 1997
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

#define _ALLOC_OPERATOR_NEW_STL_
#ifdef _ALLOC_OPERATOR_NEW_STL_

#include "forward.h"
#include "config_def.h"

template<class _Ty>
class __Default_alloc {
public:
    using value_type = _Ty;
    using pointer = _Ty*;
    using const_pointer = const _Ty*;
    using void_pointer = void*;
    using const_void_pointer = const void*;

    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;


    CCC_CONSTEXPR_CXX17 size_type max_size() const CCC_NOEXCEPT {
        return size_type(-1) / sizeof(_Ty);
    }

    //* Show warn for exist desert exp 
    #if CCC_VERSION > 20
    [[nodiscard]] CCC_CONSTEXPR_CXX23
        __allocation_result<_Ty> allocate_at_least(size_type __n) {
        return { allocate(__n), __n };
    }
    #endif

    CCC_CONSTEXPR_CXX17 void deallocate(pointer __ptr, const size_type __n) {
        if (__ptr != nullptr) ::operator delete(__ptr);
    }
    CCC_CONSTEXPR_CXX17 void deallocate(pointer __ptr) {
        if (__ptr != nullptr) ::operator delete(__ptr);
    }


    template <class _Objty, class... _Args>
    CCC_CONSTEXPR_CXX17 void construct(_Objty* const __p, _Args &&...__args)
        CCC_NOEXCEPT(CCC_NOEXCEPT(::new (_Voidify_iter(__p))_Objty(FVstd::forward<_Args>(__args)...))) {
        ::new ((void*)__p)_Objty(FVstd::forward<_Args>(__args)...);
    }

    template<class _Tp>
    CCC_CONSTEXPR_CXX17 void destroy(_Tp* __res) {
        &(*__res)->~_Tp();
    }

    CCC_CONSTEXPR_CXX17 pointer allocate(size_type __n) {
        return 0 == __n ? nullptr :
            static_cast<pointer>(::operator new(__n * sizeof(value_type)));
    }

    CCC_CONSTEXPR_CXX17 pointer allocate(void) {
        return static_cast<pointer>(::operator new(sizeof(value_type)));
    }

};

template<class _Ty>
class __Default_alloc<const _Ty> {
public:
    using value_type = const _Ty;

    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;


    CCC_CONSTEXPR_CXX20 __Default_alloc() CCC_NOEXCEPT = default;

    template<class _Tp>
    CCC_CONSTEXPR_CXX20 __Default_alloc(const __Default_alloc<_Tp>&) CCC_NOEXCEPT {};

    //* Show warn for exist desert exp 
    #if CCC_VERSION > 20
    [[nodiscard]] CCC_CONSTEXPR_CXX23
        __allocation_result<const _Ty> allocate_at_least(const size_type __n) {
        return { allocate(__n), __n };
    }
    #endif

    CCC_CONSTEXPR_CXX17 void deallocate(const _Ty* __ptr, const size_type __n) {
        if (__ptr != nullptr) ::operator delete(const_cast<_Ty*>(__ptr));
    }
    CCC_CONSTEXPR_CXX17 void deallocate(const _Ty* __ptr) {
        if (__ptr != nullptr) ::operator delete(const_cast<_Ty*>(__ptr));
    }


    template <class _Objty, class... _Args>
    CCC_CONSTEXPR_CXX17 void construct(_Objty* const __p, _Args &&...__args)
        CCC_NOEXCEPT(CCC_NOEXCEPT(::new (_Voidify_iter(__p))_Objty(FVstd::forward<_Args>(__args)...))) {
        ::new ((void*)__p)_Objty(FVstd::forward<_Args>(__args)...);
    }

    template<class _Tp>
    CCC_CONSTEXPR_CXX17 void destroy(_Tp* __res) {
        &(*__res)->~_Tp();
    }

    const _Ty* allocate(size_type __n) {
        return 0 == __n ? nullptr :
            const_cast<_Ty*>(::operator new(__n * sizeof(value_type)));
    }
    const _Ty* allocate(void) { const_cast<_Ty*>(::operator new(sizeof(value_type))); }

    const size_type max_size() const CCC_NOEXCEPT { return size_type(~0) / sizeof(_Ty); }

};

template<class _Ty>
class __SubDefault_alloc
    : public __Default_alloc<_Ty> {};


#endif      //* _ALLOC_OPERATOR_NEW_STL_
