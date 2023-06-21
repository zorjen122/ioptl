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

#define _ALLOCATOR_TRAITS_STL_
#ifdef _ALLOCATOR_TRAITS_STL_

#include "tycommom.h"
#include "allocator_stl.h"
#include "forward.h"
#include "config_def.h"

using namespace MPL_L;


#define CCC_ALLOCATOR_TRAITS_TYPE_XXX(NAME,PROPERTIES);                     \
template<class _Ty, class = void> struct NAME: false_type {};               \
template<class _Ty>                                                         \
struct NAME <_Ty, __void_t<typename _Ty::PROPERTIES>>:true_type {};         \

namespace CCC_STL {
//* using traits feature 
    template<class _Ty, class _Alloc>   struct allocator_traits_impl {};

    CCC_ALLOCATOR_TRAITS_TYPE_XXX(_has_pointer, pointer);

    template<class _Ty, class _Alloc,
        /*class _RawAlloc = MPL_L::remove_reference_t<_Alloc>, */
        bool = _has_pointer<_Alloc>::value> struct __pointer {
        using type = typename _Alloc::pointer;
    };

    template<class _Ty, class _Alloc>
    struct __pointer<_Ty, _Alloc, false> { using type = _Ty*; };


    CCC_ALLOCATOR_TRAITS_TYPE_XXX(_has_const_pointer, const_pointer);

    template<class _Ty, class _Alloc,
        bool = _has_const_pointer<_Alloc>::value > struct __const_pointer {
        using type = typename _Alloc::const_pointer;
    };

    template<class _Ty, class _Alloc>
    struct __const_pointer<_Ty, _Alloc, false> { using type = const _Ty*; };



    CCC_ALLOCATOR_TRAITS_TYPE_XXX(_has_void_pointer, void_pointer);

    template<class _Ty, class _Alloc,
        bool = _has_void_pointer<_Alloc>::value > struct __void_pointer {
        using type = typename _Alloc::void_pointer;
    };
    template<class _Ty, class _Alloc>
    struct __void_pointer<_Ty, _Alloc, false> { using type = void*; };


    CCC_ALLOCATOR_TRAITS_TYPE_XXX(_has_const_void_pointer, const_void_pointer);

    template<class _Ty, class _Alloc,
        bool = _has_const_void_pointer<_Alloc>::value > struct __const_void_pointer {
        using type = typename _Alloc::const_void_pointer;
    };
    template<class _Ty, class _Alloc>
    struct __const_void_pointer<_Ty, _Alloc, false> { using type = const void*; };


    CCC_ALLOCATOR_TRAITS_TYPE_XXX(_has_size_type, size_type);

    template<class _Ty, class _Alloc,
        bool = _has_size_type<_Alloc>::value > struct __size_type { using type = ::std::size_t; };

    template<class _Ty, class _Alloc>
    struct __size_type<_Ty, _Alloc, false> { using type = typename _Alloc::size_type; };


    CCC_ALLOCATOR_TRAITS_TYPE_XXX(_has_alloc_traits_difference_type, difference_type);

    template<class _Ty, class _Alloc,
        bool = _has_alloc_traits_difference_type<_Alloc>::value > struct __difference_type {
        using type = typename _Alloc::difference_type;
    };


    template<class _Ty, class _Alloc>
    struct __difference_type<_Ty, _Alloc, true> { using type = typename _Alloc::difference_type; };


    template<class _Ty, class _Tp, class = void>struct _has_rebind_other :false_type {};
    template<class _Ty, class _Tp>struct _has_rebind_other<_Ty, _Tp,
        __void_t<typename _Ty::template rebind<_Tp>::other>> :true_type{};


    template<class _Alloc, class _Tp, bool = _has_rebind_other<_Alloc, _Tp>::value>
    struct __allocator_traits_rebind {
        static_assert(_has_rebind_other<_Alloc, _Tp>::value, "This allocator has to implement rebind");
        using type = typename _Alloc::template rebind<_Tp>::other;
    };

    /*
      * For allocator Args of the frist class-parameter _Alloc whole
      * is allocator.h of the Alloc.h file, which maybe is
      * allocator<_Ty,_Alloc> -> allocator<_Ty> question. but now that non-change.
      */
    template<template<class, class, class...>class _Alloc, class _Tb, class _Ty, class _Tp, class... _Args >
    struct __allocator_traits_rebind <_Alloc<_Tb, _Ty, _Args...>, _Tp, false> {
        using type = typename _Alloc<_Ty, _Args...>::template rebind<_Tp>::other;
    };

    template<template<class, class, class...>class _Alloc, class _Tb, class _Ty, class _Tp, class... _Args >
    struct __allocator_traits_rebind <_Alloc<_Tb, _Ty, _Args...>, _Tp, true> {
        using type = _Alloc<_Tb, _Tp, _Args...>;
    };

    template<class _Alloc, class _Tp>
    using __allocator_traits_rebind_t = typename __allocator_traits_rebind<_Alloc, _Tp>::type;


    template<class, class _Alloc, class... _Args>
    struct _has_construct_impl : false_type {};
    template<class _Alloc, class... _Args>
    struct _has_construct_impl<
        decltype(MPL_L::declval<_Alloc>().construct(declval<_Args>()...)),
        _Alloc, _Args...>
        : true_type {};

    template<class _Alloc, class... _Args>
    struct _has_construct : _has_construct_impl<void, _Alloc, _Args...> {};


    template<class _Alloc, class _Ptr, class = void>
    struct _has_destroy_impl : false_type {};
    template<class _Alloc, class _Ptr>
    struct _has_destroy_impl <_Alloc, _Ptr, decltype(MPL_L::declval<_Alloc>().destroy(declval<_Ptr>()))>
        :true_type {};

    template<class _Alloc, class _Ptr>
    struct _has_destroy : _has_destroy_impl<_Alloc, _Ptr, void> {};


    template<class _Ty, class _Alloc>
    struct allocator_traits {
    public:
        using allocator_type = CCC_STL::allocator<_Ty, _Alloc>;

        using value_type = typename allocator_type::value_type;
        using pointer = typename __pointer<value_type, allocator_type>::type;
        using const_pointer = typename __const_pointer<pointer, allocator_type>::type;
        using void_pointer = typename __void_pointer<pointer, allocator_type>::type;
        using const_void_pointer = typename __const_void_pointer<pointer, allocator_type>::type;
        using difference_type = typename __difference_type<pointer, allocator_type>::type;
        using size_type = typename __size_type<difference_type, allocator_type>::type;


        CCC_CONSTEXPR_CXX17 allocator_traits() CCC_NOEXCEPT = default;
        CCC_CONSTEXPR_CXX17  allocator_traits(const allocator_traits<_Ty, _Alloc>&) CCC_NOEXCEPT {};


        //* Show warn for exist desert exp 
        #if CCC_VERSION > 20
        [[nodiscard]] CCC_CONSTEXPR_CXX23
            allocation_result<_Ty> allocate_at_least(size_t __n) {
            return { _Alloc::allocate(__n), __n };
        }
        #endif

        template<class _Tp, class _Up,
            class = enable_if_t<_has_rebind_other<allocator_type, _Tp, _Up>::value>>
            struct rebind { using other = allocator_traits<_Tp, _Up>; };


        CCC_CONSTEXPR_CXX17 size_type max_size() const CCC_NOEXCEPT { return size_type(-1) / sizeof(_Ty); }


        CCC_CONSTEXPR_CXX17 pointer allocate(size_type __n) {
            return (0 == __n) ? nullptr : _Alloc::allocate(__n * sizeof(value_type));
        }
        CCC_CONSTEXPR_CXX17 pointer allocate(void) {
            return _Alloc::allocate(sizeof(value_type));
        }

        CCC_CONSTEXPR_CXX17 void deallocate(pointer __p, size_type __n) {
            if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(value_type));
        }
        CCC_CONSTEXPR_CXX17 void deallocate(pointer __p) {
            _Alloc::deallocate(__p, sizeof(value_type));
        }


        template <class _Objty, class... _Args,
            class = enable_if_t<_has_construct<allocator_type, _Args...>::value>>
            CCC_CONSTEXPR_CXX17 void construct(_Objty* const __p, _Args &&...__args)
            CCC_NOEXCEPT(CCC_NOEXCEPT(::new (_Voidify_iter(__p))_Objty(FVstd::forward<_Args>(__args)...))) {
            ::new (_Voidify_iter(__p))_Objty(FVstd::forward<_Args>(__args)...);
        }

        template<class _Iter, class = enable_if_t<_has_destroy<allocator_type, _Iter>::value>>
        CCC_CONSTEXPR_CXX17 void destroy(_Iter __res) { &(*__res)->~_Iter(); }
    };
};

#endif          //* _ALLOCATOR_TRAITS_STL_
