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

#define _ITERATOR_BASE_STL_
#ifdef _ITERATOR_BASE_STL_

#include "deftype.h"        
#include "algobase_stl.h"
#include "forward.h"
#include "type_traits_stl.h"
#include "tycommom.h"
#include "config_def.h"

using namespace Traits_type;
using namespace MPL_L;

#include <new>      
#include <utility>
#include <assert.h>

namespace CCC_STL {

    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template<class _Category,
        class _Ty,
        class _Distance = ::std::ptrdiff_t,
        class _Pointer = _Ty*,
        class _Reference = _Ty&>
    class iterator {
    public:
        using iteartor_category = _Category;

        using value_type = _Ty;
        using pointer = _Ty*;
        using reference = _Ty&;
        using difference_type = _Distance;
    };


    // I/O/F/B/R iterator
    template<class _Ty, class _Distance>
    struct input_iterator {
        using iteartor_category = input_iterator_tag;

        using value_type = _Ty;
        using pointer = _Ty*;
        using reference = _Ty&;
        using difference_type = _Distance;
    };


    struct output_iterator {
        using iteartor_category = output_iterator_tag;

        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
    };


    template<class _Ty, class _Distance>
    struct forward_iterator : public input_iterator<_Ty, _Distance> {
        using iteartor_category = forward_iterator_tag;

        using value_type = _Ty;
        using pointer = _Ty*;
        using reference = _Ty&;
        using difference_type = _Distance;
    };


    template<class _Ty, class _Distance>
    struct bidirectional_iterator : public forward_iterator<_Ty, _Distance> {
        using iteartor_category = bidirectional_iterator_tag;

        using value_type = _Ty;
        using pointer = _Ty*;
        using reference = _Ty&;
        using difference_type = _Distance;

    };


    template<class _Ty, class _Distance>
    struct random_access_iterator : public bidirectional_iterator<_Ty, _Distance> {
        using iteartor_category = random_access_iterator_tag;

        using value_type = _Ty;
        using pointer = _Ty*;
        using reference = _Ty&;
        using difference_type = _Distance;
    };


    template <class _Iterator>
    struct iterator_traits {
        using iterator = typename _Iterator::iterator_category;

        using value_type = typename _Iterator::value_type;
        using pointer = typename _Iterator::pointer;
        using reference = typename _Iterator::reference;
        using difference = typename _Iterator::difference_type;
    };


    template <class _Tp>
    struct iterator_traits<_Tp*> {
        using iterator_category = random_access_iterator_tag;

        using value_type = _Tp;
        using difference_type = ::std::ptrdiff_t;
        using pointer = _Tp*;
        using reference = _Tp&;
    };


    template <class _Tp>
    struct iterator_traits<const _Tp*> {
        using iterator_category = random_access_iterator_tag;

        using value_type = _Tp;
        using difference_type = ::std::ptrdiff_t;
        using pointer = const _Tp*;
        using reference = const _Tp&;
    };


    template<class _Iterty>
    using difference_type_t = typename iterator_traits<_Iterty>::difference_type;

    // #elif __cplusplus >= 202003L

    template<class _Iter>
    struct iterator_category { using type = typename iterator_traits<_Iter>::iterator_category; };

    template<class _Ty>
    concept input_iterator_tag_cond = ::std::is_base_of<input_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>::value;

    template<class _Ty>
    concept forward_iterator_tag_cond = ::std::is_base_of<forward_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>::value;

    template<class _Ty>
    concept output_iterator_tag_cond = ::std::is_base_of<output_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>::value;

    template<class _Ty>
    concept bidirectional_iterator_tag_cond = ::std::is_base_of<bidirectional_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>::value;

    template<class _Ty>
    concept random_access_iterator_tag_cond = ::std::is_base_of<random_access_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>::value;

    template<class _Ty>
    concept poly_forward_iterator_tag_cond = ::std::is_base_of<forward_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>::value;

    template<class _Ty>
    concept poly_random_access_iterator_tag_cond = ::std::is_base_of<input_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>::value;

    template<class _Ty>
    concept poly_bidirectional_iterator_tag_cond = ::std::is_base_of<random_access_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>::value;


    // template<class _Ty>
    // concept input_iterator_tag_cond = MPL_L::is_same<typename iterator_category<_Ty>::
    //     type, input_iterator_tag>::value;

    // template<class _Ty>
    // concept forward_iterator_tag_cond = MPL_L::is_same<typename iterator_category<_Ty>::
    //     type, forward_iterator_tag>::value;

    // template<class _Ty>
    // concept output_iterator_tag_cond = MPL_L::is_same<typename iterator_category<_Ty>::
    //     type, output_iterator_tag>::value;

    // template<class _Ty>
    // concept bidirectional_iterator_tag_cond = MPL_L::is_same<typename iterator_category<_Ty>::
    //     type, bidirectional_iterator_tag>::value;

    // template<class _Ty>
    // concept random_access_iterator_tag_cond = MPL_L::is_same<typename iterator_category<_Ty>::
    //     type, random_access_iterator_tag>::value;

    // template<class Iterator_tag>
    // concept poly_forward_iterator_tag_cond = forward_iterator_tag_cond<Iterator_tag>
    //     || bidirectional_iterator_tag_cond<Iterator_tag>
    //     || random_access_iterator_tag_cond<Iterator_tag>;

    // template<class Iterator_tag>
    // concept poly_random_access_iterator_tag_cond = input_iterator_tag_cond<Iterator_tag>
    //     || forward_iterator_tag_cond<Iterator_tag>
    //     || bidirectional_iterator_tag_cond<Iterator_tag>
    //     || random_access_iterator_tag_cond<Iterator_tag>;

    // template<class Iterator_tag>
    // concept poly_bidirectional_iterator_tag_cond = bidirectional_iterator_tag_cond<Iterator_tag>
    //     || random_access_iterator_tag_cond<Iterator_tag>;




    template<class _IterStart, class _IterDistance>
    CCC_CONSTEXPR_CXX17 void __advance(_IterStart& __i, _IterDistance __n)CCC_NOEXCEPT
        requires input_iterator_tag_cond<_IterStart> {
        while (--__n) ++__i;
    }

    template<output_iterator_tag_cond _Ty, class _IterDistance>
    CCC_CONSTEXPR_CXX17 void __advance(_Ty& __i, _IterDistance __n) CCC_NOEXCEPT {}

    template<poly_bidirectional_iterator_tag_cond _Ty, class _IterDistance>
    CCC_CONSTEXPR_CXX17 void __advance(_Ty& __i, _IterDistance __n) CCC_NOEXCEPT {
        // operator--() so n == 0 to breack
        if (__n >= 0)
            while (__n--) ++__i;
        else
            while (__n++) --__i;
    }

    template<random_access_iterator_tag_cond _Ty, class _IterDistance>
    CCC_CONSTEXPR_CXX17 void __advance(_Ty& __i, _IterDistance __n) CCC_NOEXCEPT {
        __i += __n;
    }


    template <poly_random_access_iterator_tag_cond _Ty>
    CCC_CONSTEXPR_CXX17 typename iterator_traits<_Ty>::difference_type
        __distance(_Ty __frist, _Ty __last)CCC_NOEXCEPT
    {
        typename iterator_traits<_Ty>::difference_type __n = 0;
        while (__frist != __last) {
            ++__frist; ++__n;
        }
        return __n;
    }


    template <random_access_iterator_tag_cond _Ty>
    CCC_CONSTEXPR_CXX17 difference_type_t<_Ty>
        __distance(_Ty __frist, _Ty __last) CCC_NOEXCEPT { return __last - __frist; }

    template<poly_random_access_iterator_tag_cond _Ty>
    CCC_CONSTEXPR_CXX17 difference_type_t<_Ty>
        _distance(_Ty __frist, _Ty __last) CCC_NOEXCEPT {
        difference_type_t<_Ty> __count = 0;
        for (;__frist != __last;++__frist) ++__count;
        return __count;
    }

    template<random_access_iterator_tag_cond _Ty>
    CCC_CONSTEXPR_CXX17 difference_type_t<_Ty>
        distance(_Ty __frist, _Ty __last) CCC_NOEXCEPT {
        return static_cast<difference_type_t<_Ty>>(__frist - __last);
    }


    template<poly_bidirectional_iterator_tag_cond _Ty>
    CCC_CONSTEXPR_CXX17 _Ty __copy__backward(_Ty __frist, _Ty __last, _Ty __res) CCC_NOEXCEPT {
        if (__frist != __last)
            *(--__res) = *(--__last);
        return __res;
    }

    template<random_access_iterator_tag_cond _Ty, class _Distance>
    CCC_CONSTEXPR_CXX17 _Ty __copy__backward(_Ty __frist, _Ty __last, _Ty __res) CCC_NOEXCEPT {
        for (_Distance __n = __last - __frist;__n > 0;--__n)
            *(--__res) = *(--__last);
        return __res;
    }

    template<poly_random_access_iterator_tag_cond _Iterator>
    CCC_CONSTEXPR_CXX17 _Iterator copy_backward
    (_Iterator __frist, _Iterator __last, _Iterator __res) CCC_NOEXCEPT {

        return __copy__backward(__frist, __last, __res);
    }


    /*
      *- Error new initializer expression list treated as compound expression
      *  CCC_NOEXCEPT(CCC_NOEXCEPT(::new (_Voidify_iter(__ptr))
      *  _Objty(FVstd::forward<_Args>(__args)...)))
      *
      *-  New case: only ::new to end-element
      */

    template <class _Objty, class... _Args>
    CCC_CONSTEXPR_CXX17 void construct(_Objty* __ptr, _Args&&... __val) {
        (..., ::new (_Voidify_iter(__ptr)) _Objty(__val));
    };

    template <class _Objty>
    CCC_CONSTEXPR_CXX17 void construct(_Objty* const __ptr)
        CCC_NOEXCEPT(std::is_nothrow_constructible_v<_Objty>)
        /*CCC_NOEXCEPT(CCC_NOEXCEPT(::new (_Voidify_iter(__ptr))_Objty())) */ {
        assert(__ptr != nullptr);
        ::new (_Voidify_iter(__ptr)) _Objty();
    }

    template<class _Ty>
    CCC_CONSTEXPR_CXX17 void _destroy(_Ty* const __res) { __res->~_Ty(); }

    template<class _Iter>
    CCC_CONSTEXPR_CXX17 void destroy(_Iter __frist, _Iter __last) {
        for (;__frist != __last;++__frist)
            _destroy(&*__frist);
    }

    template<class _Iter>
    CCC_CONSTEXPR_CXX17 void destroy(_Iter __res) { _destroy(&*__res); }


    /*
      *-Uninitailzed function
      *  For all iterator_cond we need conduct about a iterator category tag to iterator_traits.
      *  template<forward_iterator_tag_cond _Ty, class _Tv>
      */

    template<class _Ty, class _Tv>
    CCC_CONSTEXPR_CXX17 void uninitialized_fill(_Ty __frist, _Tv __val) {
        fill(__frist, __val);
    }

    // template<forward_iterator_tag_cond _Ty, class _Size, class _Tv>
    template<class _Ty, class _Size, class _Tv>
    CCC_CONSTEXPR_CXX17 void uninitialized_fill_n(_Ty __frist, _Size __n, _Tv __val) {
        fill_n(__frist, __n, __val);
    }


    //* template<input_iterator_tag_cond _Ty, forward_iterator_tag_cond _Tp>
    template<class _Ty, class _Tp>
    _Tp
        uninitalized_copy(_Ty __frist, _Ty __last, _Tp __iter_data) {
        _Tp res = __iter_data;

        try {
            for (;__frist != __last; ++res, ++__frist)
                construct(&*res, *__frist);
            return res;
        }
        catch (...) {
            destroy(__iter_data, res);
            throw;
        }

    }


    template<input_iterator_tag_cond _Ty, class _Size, forward_iterator_tag_cond _Tp>
    ::std::pair<_Ty, _Tp>
        uninitalized_copy_n(_Ty __frist, _Size __n, _Tp __iter_data) {
        _Tp res = __iter_data;

        try {
            for (;__n > 0; --__n, ++res, ++__frist)
                construct(&*res, *__frist);
            return ::std::pair<_Ty, _Tp>(::std::move(__frist), ::std::move(res));
        }
        catch (...) {
            destroy(__iter_data, res);
        }
    }

}

#endif          //*  _ITERATOR_BASE_STL_
