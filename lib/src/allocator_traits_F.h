/*
* Copyright(c) 1997 - zorjen122
* Silicon Graphics Computer Systems, Inc.
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
*that both that copyright notice and this permission notice appear
* in supporting documentation.Silicon Graphics makes no
* representations about the suitability of this software for any
* purpose.It is provided "as is" without express or implied warranty.
*/


#pragma once

#define _ALLOCATOR_TRAITS_IOP_
#ifdef _ALLOCATOR_TRAITS_IOP_

#include "type_trait.h"
#include "metacomponent.h"
#include "allocator_F.h"
#include "Utility.h"
#include "config_def.h"

using namespace MPL_L;


#define IOP_ALLOCATOR_TRAITS_TYPE_XXX(NAME,PROPERTIES);                     \
template<class _Ty, class = void> struct NAME: false_type {};               \
template<class _Ty>                                                         \
struct NAME <_Ty, __void_t<typename _Ty::PROPERTIES>>:true_type {};         \

namespace IOP_STL {
//* using traits feature
    template<class _Ty, class _Alloc>   struct allocator_traits_impl {};

    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_pointer, pointer);

    template<class _Ty, class _Alloc,
        /*class _RawAlloc = MPL_L::remove_reference_t<_Alloc>, */
        bool = _has_pointer<_Alloc>::value > struct __pointer {
        using type = typename _Alloc::pointer;
    };

    template<class _Ty, class _Alloc>
    struct __pointer<_Ty, _Alloc, false> { using type = _Ty*; };


    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_const_pointer, const_pointer);

    template<class _Ty, class _Alloc,
        bool = _has_const_pointer<_Alloc>::value > struct __const_pointer {
        using type = typename _Alloc::const_pointer;
    };

    template<class _Ty, class _Alloc>
    struct __const_pointer<_Ty, _Alloc, false> { using type = const _Ty*; };


    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_void_pointer, void_pointer);

    template<class _Ty, class _Alloc,
        bool = _has_void_pointer<_Alloc>::value > struct __void_pointer {
        using type = typename _Alloc::void_pointer;
    };

    template<class _Ty, class _Alloc>
    struct __void_pointer<_Ty, _Alloc, false> { using type = void*; };


    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_const_void_pointer, const_void_pointer);

    template<class _Ty, class _Alloc,
        bool = _has_const_void_pointer<_Alloc>::value > struct __const_void_pointer {
        using type = typename _Alloc::const_void_pointer;
    };

    template<class _Ty, class _Alloc>
    struct __const_void_pointer<_Ty, _Alloc, false> { using type = const void*; };


    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_size_type, size_type);

    template<class _Ty, class _Alloc,
        bool = _has_size_type<_Alloc>::value > struct __size_type { using type = ::std::size_t; };

    template<class _Ty, class _Alloc>
    struct __size_type<_Ty, _Alloc, false> { using type = typename _Alloc::size_type; };


    IOP_ALLOCATOR_TRAITS_TYPE_XXX(_has_alloc_traits_difference_type, difference_type);

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


    //* For ex: one-allocator to mult-allocator, mult-allocator op(iterator) : next/prev ... 
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
    struct _has_construct_impl
        < decltype(MPL_L::declval<_Alloc>().construct(declval<_Args>()...)),
        _Alloc, _Args... > : true_type {};

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
        using allocator_type = _Alloc;
        using allocator_traits_type = allocator_traits<_Ty, allocator_type>;

        using value_type = typename allocator_type::value_type;
        using pointer = typename __pointer<value_type, allocator_type>::type;
        using const_pointer = typename __const_pointer<pointer, allocator_type>::type;
        using void_pointer = typename __void_pointer<pointer, allocator_type>::type;
        using const_void_pointer = typename __const_void_pointer<pointer, allocator_type>::type;
        using difference_type = typename __difference_type<pointer, allocator_type>::type;
        using size_type = typename __size_type<difference_type, allocator_type>::type;


        IOP_CONSTEXPR_CXX17 allocator_traits() NOEXCEPT = default;
        IOP_CONSTEXPR_CXX17  allocator_traits(const allocator_traits<_Ty, _Alloc>&) NOEXCEPT {};


        #if IOP_VERSION > 20
        [[nodiscard]] IOP_CONSTEXPR_CXX23
            allocation_result<_Ty> allocate_at_least(size_t __n) {
            return { _Alloc::allocate(__n), __n };
        }
        #endif

        template<class _Tp, class _Up,
            class = enable_if_t<_has_rebind_other<allocator_type, _Tp, _Up>::value>>
            struct rebind { using other = allocator_traits<_Tp, _Up>; };


        IOP_CONSTEXPR_CXX17 size_type max_size() const NOEXCEPT { return size_type(-1) / sizeof(_Ty); }


        IOP_CONSTEXPR_CXX17 pointer allocate(size_type __n) {
            return (0 == __n) ? nullptr : _Alloc::allocate(__n * sizeof(value_type));
        }
        IOP_CONSTEXPR_CXX17 pointer allocate(void) {
            return _Alloc::allocate(sizeof(value_type));
        }

        IOP_CONSTEXPR_CXX17 void deallocate(pointer __p, size_type __n) {
            if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(value_type));
        }
        IOP_CONSTEXPR_CXX17 void deallocate(pointer __p) {
            _Alloc::deallocate(__p, sizeof(value_type));
        }


        #if IOP_VERSION > 17

        template<class _Objty, class... _Args,
            class = decltype(::new(declval<void*>()) _Objty(declval<_Args>()...))
        >
        IOP_CONSTEXPR_CXX17 void construct_at(_Objty* __p, _Args&&...__args)
            NOEXCEPT(std::is_nothrow_constructible_v<_Objty>) {
            ::new (_Voidify_ptr(__p)) _Objty(FVstd::forward<_Args>(__args)...);
        }

        template<class _Objty, class... _Args,
            class = enable_if_t<_has_construct<allocator_type, _Args...>::value>>
            IOP_CONSTEXPR_CXX17 void construct(allocator_type&, _Objty* __p, _Args&&... __args) {
            construct_at(__p, FVstd::forward<_Args>(__args)...);
        }

        template<class _Objty, class... _Args,
            class = enable_if_t<_has_construct<allocator_type, _Args...>::value>>
            IOP_CONSTEXPR_CXX17 void construct(_Objty* __p, _Args&&... __args) {
            construct_at(__p, FVstd::forward<_Args>(__args)...);
        }

        #else

        template <class _Objty, class... _Args>
        IOP_CONSTEXPR_CXX17 void construct(allocator_type& __a, _Objty* __p, _Args&&... __args) {
            __a.construct(__p, FVstd::forward<_Args>(__args)...);
        }

        template <class _Objty, class... _Args>
        IOP_CONSTEXPR_CXX17 void construct(_Objty* __p, _Args&&... __args)
            NOEXCEPT(std::is_nothrow_constructible_v<_Objty>) {
            _Alloc::construct(__p, FVstd::forward<_Args>(__args)...);
        }

        #endif


        template<class _Iter, class = enable_if_t<_has_destroy<allocator_type, _Iter>::value>>
        IOP_CONSTEXPR_CXX17 void destroy(_Iter __res)
            NOEXCEPT(NOEXCEPT(_Alloc::destroy(__res))) {
            _Alloc::destroy(__res);
        }

        template<class _Iter, class = enable_if_t<_has_destroy<allocator_type, _Iter>::value>>
        IOP_CONSTEXPR_CXX17 void destroy(allocator_type& __a, _Iter __res)
            NOEXCEPT(NOEXCEPT(__a.destroy(__res))) {
            __a.destroy(__res);
        }

    };

    //* For allocator_traits warrpper( self(is-a)-type-obj )
        // Ex: obj => { move(fun_ch_t(obj)) | Error as fun_ch_t retrun lvalue-type } 
    template<class _Ty, class _Alloc>
    class alloc_traits : public allocator_traits<_Ty, _Alloc> {
    public:
        using allocator_type = _Alloc;
        using Base_type = allocator_traits<_Ty, allocator_type>;

        using value_type = typename Base_type::value_type;
        using pointer = typename Base_type::pointer;
        using const_pointer = typename Base_type::const_pointer;
        using void_pointer = typename Base_type::void_point;
        using const_void_pointer = typename Base_type::const_void_pointer;
        using difference_type = typename Base_type::difference_type;
        using size_type = typename Base_type::size_type;

        using Base_type::allocate_at_least;
        using Base_type::allocate;
        using Base_type::construct;
        using Base_type::deallocate;
        using Base_type::destroy;
        using Base_type::max_size;

    private:

        template<class _Ptr>
        using __is_custom_pointer = MPL_L::oper::and_<MPL_L::is_same<pointer, _Ptr>,
            MPL_L::oper::not_<MPL_L::is_pointer<_Ptr>>>;

    public:

        template<class _Tp, class _Up,
            class = enable_if_t<_has_rebind_other<allocator_type, _Tp, _Up>::value>>
            struct rebind { using other = typename Base_type::template rebind<_Tp, _Up>::other; };

        #if IOP_VERSION > 17

        template <class _Objty, class... _Args,
            class = enable_if_t<_has_construct<allocator_type, _Args...>::value>
        >
        IOP_CONSTEXPR_CXX17 enable_if_t<__is_custom_pointer<_Alloc>::value>
            construct(allocator_type& __a, _Objty* __p, _Args &&...__args)
            NOEXCEPT(std::is_nothrow_constructible_v<_Objty>) {
            Base_type::construct(std::to_address(__p), FVstd::forward<_Args>(__args)...);
        }

        template< class _Iter, class = enable_if_t<_has_destroy<allocator_type, _Iter>::value>>
        IOP_CONSTEXPR_CXX20 enable_if_t<__is_custom_pointer<_Alloc>::value>
            destroy(allocator_type& __a, _Iter __res)
            NOEXCEPT(NOEXCEPT(Base_type::destroy(__a, ::std::to_address(__res)))) {
            Base_type::destroy(__a, ::std::to_address(__res));
        }

        #else

        template < class _Objty, class... _Args,
            class = enable_if_t<_has_construct<allocator_type, _Args...>::value>>
            IOP_CONSTEXPR_CXX17 enable_if_t<__is_custom_pointer<_Alloc>::value
            >
            construct(allocator_type& __a, _Objty* __p, _Args &&...__args)
            NOEXCEPT(std::is_nothrow_constructible_v<_Objty>) {
            Base_type::construct(__a, &*__p, FVstd::forward<_Args>(__args)...);
        }

        template< class _Iter, class = enable_if_t<_has_destroy<allocator_type, _Iter>::value>>
        IOP_CONSTEXPR_CXX20 enable_if_t<__is_custom_pointer<_Alloc>::value>
            destroy(allocator_type& __a, _Iter __res)
            NOEXCEPT(NOEXCEPT(Base_type::destroy(__a, &*__res))) {
            Base_type::destroy(__a, &*__res));
        }
        #endif


    };

};

#endif          //* _ALLOCATOR_TRAITS_IOP_
