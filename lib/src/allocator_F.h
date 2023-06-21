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


/*
 *- Allocator in other complie(MSVC LLVM) phenomenon
 *  is non-function-construct to alloc of package : ::operator new ...
 */

#pragma once

#define _ALLOCATOR_IOP_
#ifdef  _ALLOCATOR_IOP_

#include "Utility.h"
#include "type_trait.h"
#include "metacomponent.h"
#include "config_def.h"
#include "allocator_result.h"

#include <assert.h>
#include <utility>


using namespace MPL_L;

namespace IOP_STL {


    template<class _Ty, class _Alloc>class allocator;

    template<class _Alloc>class allocator<void, _Alloc> {
    public:
        using pointer = void*;
        using const_pointer = const void*;
        using value_type = const void;

        template<class _Tp, class _Up>struct rebind { using other = allocator<_Tp, _Up>; };
    };

    template<class _Alloc>class allocator<const void, _Alloc> {
    public:
        using pointer = const void*;
        using const_pointer = const void*;
        using value_type = const void;

        template<class _Tp, class _Up>struct rebind { using other = allocator<_Tp, _Up>; };
    };

    /*
     *- reference: MSVC STL
     *  For everyclass, this meta-function derive if that's base class
     *  So the same A,B class will not conflict for type.
     */
    template<bool _Cond, class _Unique>
    struct _non_trivial_if {};

    template<class _Unique>
    struct _non_trivial_if<true, _Unique> {
        _non_trivial_if() NOEXCEPT {};
    };


    template<class _Ty, class _Alloc>
    class allocator<const _Ty, _Alloc>
        : private _non_trivial_if <!is_void<_Ty>::value, allocator<_Ty, _Alloc>> {
        static_assert(!is_volatile<_Ty>::value, "IOP_STL::allocator does not support volatile types");

    public:
        using value_type = const _Ty;
        using size_type = ::std::size_t;
        using difference_type = ::std::ptrdiff_t;

        IOP_CONSTEXPR allocator() NOEXCEPT = default;
        IOP_CONSTEXPR allocator(const allocator<_Ty, _Alloc>&) NOEXCEPT {}


        //* Show warn for exist desert exp 
        #if IOP_VERSION > 20
        [[nodiscard]] IOP_CONSTEXPR_CXX23
            __allocation_result<_Tp> allocate_at_least(size_type __n) {
            return _Alloc::allocate_at_least(__n);
        }
        #endif

        template<class _Tp, class _Up>struct rebind { using other = allocator<_Tp, _Up>; };

        IOP_CONSTEXPR_CXX17 value_type* allocate(size_type __n) {
            return 0 == __n ?
                0 : _Alloc::allocate(__n * sizeof(value_type));
        }

        IOP_CONSTEXPR_CXX17 value_type* allocate(void) { return _Alloc::allocate(sizeof(value_type)); }

        IOP_CONSTEXPR_CXX17 void deallocate(value_type* __p, size_type __n) {
            if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(value_type));
        }
        IOP_CONSTEXPR_CXX17 void deallocate(value_type* __p) {
            _Alloc::deallocate(__p, sizeof(value_type));
        }


        template <class _Objty, class... _Args>
        IOP_CONSTEXPR_CXX17 void construct(_Objty* const __p, _Args &&...__args)
            NOEXCEPT(NOEXCEPT(::new (_Voidify_ptr(__p))_Objty(FVstd::forward<_Args>(__args)...))) {
            ::new (_Voidify_ptr(__p))_Objty(FVstd::forward<_Args>(__args)...);
        }

        template<class _Iter>
        IOP_CONSTEXPR_CXX17 void destroy(_Iter __res) {
            &(*__res)->~_Iter();
        }

        IOP_CONSTEXPR_CXX17 size_type max_size() const NOEXCEPT {
            return size_type(-1) / sizeof(_Ty);
        }
    };


    template<class _Ty, class _Alloc>
    class allocator
        : private _non_trivial_if <!is_void<_Ty>::value, allocator<_Ty, _Alloc>> {
        static_assert(!is_volatile<_Ty>::value, "IOP_STL::allocator does not support volatile types");

    public:
        using value_type = _Ty;
        using pointer = _Ty*;
        using const_pointer = const _Ty*;
        using void_pointer = void*;
        using const_void_pointer = const void*;

        using size_type = ::std::size_t;
        using difference_type = ::std::ptrdiff_t;

        IOP_CONSTEXPR allocator() NOEXCEPT = default;
        IOP_CONSTEXPR allocator(const allocator<_Ty, _Alloc>&) NOEXCEPT {}


         //* Show warn for exist desert exp 
        #if IOP_VERSION > 20
        [[nodiscard]] IOP_CONSTEXPR_CXX23
            allocator<_Ty> allocate_at_least(size_type __n) {
            return _Alloc::allocate_at_least(__n);
        }
        #endif

        template<class _Tp, class _Up>struct rebind { using other = allocator<_Tp, _Up>; };


        IOP_CONSTEXPR_CXX17 size_type max_size() const NOEXCEPT { return size_type(~0) / sizeof(_Ty); }


        IOP_CONSTEXPR_CXX17 pointer allocate(size_type __n) {
            return 0 == __n ? 0 : _Alloc::allocate(__n * sizeof(value_type));
        }
        IOP_CONSTEXPR_CXX17 pointer allocate(void) { return _Alloc::allocate(sizeof(value_type)); }

        IOP_CONSTEXPR_CXX17 void deallocate(pointer __p, size_type __n) {
            if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(value_type));
        }
        IOP_CONSTEXPR_CXX17 void deallocate(pointer __p) {
            _Alloc::deallocate(__p, sizeof(value_type));
        }


        template <class _Objty, class... _Args>
        IOP_CONSTEXPR_CXX17 void construct(_Objty* const __p, _Args &&...__args)
            NOEXCEPT(NOEXCEPT(::new (_Voidify_ptr(__p))_Objty(FVstd::forward<_Args>(__args)...))) {
            ::new (_Voidify_ptr(__p))_Objty(FVstd::forward<_Args>(__args)...);
        }

        template<class _Iter>
        IOP_CONSTEXPR_CXX17 void destroy(_Iter __res) {
            &(*__res)->~_Iter();
        }
    };

};



#endif      //* _ALLOCATOR_IOP_