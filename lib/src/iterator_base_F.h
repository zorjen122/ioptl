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

#define _ITERATOR_BASE_IOP_
#ifdef  _ITERATOR_BASE_IOP_

#include "deftype.h"        
#include "algobase_F.h"
#include "Utility.h"
#include "type_traits_F.h"
#include "type_trait.h"
#include "metacomponent.h"
#include "config_def.h"

using namespace Traits_type;
using namespace MPL_L;

#include <new>      
#include <utility>
#include <assert.h>
#include <algorithm>

#define ITERAOTR_TAG_DEFINE(NAME,IS_A_OBJ,OBJ_TAG)                          \
    template<class _Ty, class _Distance>                                    \
    struct NAME : public IS_A_OBJ<_Ty, _Distance> {                         \
        using iteartor_category = OBJ_TAG;                                  \
        using value_type = _Ty;                                             \
        using pointer = _Ty*;                                               \
        using reference = _Ty&;                                             \
        using difference_type = _Distance;                                  \
    };


namespace IOP_STL {

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


    ITERAOTR_TAG_DEFINE(forward_iterator, input_iterator, forward_iterator_tag);
    ITERAOTR_TAG_DEFINE(bidirectional_iterator, forward_iterator, bidirectional_iterator_tag);
    ITERAOTR_TAG_DEFINE(random_access_iterator, bidirectional_iterator, random_access_iterator_tag);


    template <class _Iterator>
    struct iterator_traits {
        using iterator_category = typename _Iterator::iterator_category;

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

    template<class _Iter>
    struct iterator_category { using type = typename iterator_traits<_Iter>::iterator_category; };

    #if IOP_VERSION >= 20
    template<class _Ty>
    concept input_iterator_tag_cond = ::std::is_base_of_v<input_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>;

    template<class _Ty>
    concept forward_iterator_tag_cond = ::std::is_base_of_v<forward_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>;

    template<class _Ty>
    concept output_iterator_tag_cond = ::std::is_base_of_v<output_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>;

    template<class _Ty>
    concept bidirectional_iterator_tag_cond = ::std::is_base_of_v<bidirectional_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>;

    template<class _Ty>
    concept random_access_iterator_tag_cond = ::std::is_base_of_v<random_access_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>;


    #define _TODO_COMPATIBLE_ 0
    #elif _TODO_COMPATIBLE_ == 0
    #error "Iterators about type_traits are not yet compatible"
    #else
    template<class _Ty>
    using input_iterator_tag_cond = typename MPL_L::enable_if_t<::std::is_base_of_v<input_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>, typename iterator_traits<_Ty>::iterator_category>;

    template<class _Ty>
    using forward_iterator_tag_cond = typename MPL_L::enable_if_t<::std::is_base_of_v<forward_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>, typename iterator_traits<_Ty>::iterator_category>;

    template<class _Ty>
    using output_iterator_tag_cond = typename MPL_L::enable_if_t<::std::is_base_of_v<output_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>, typename iterator_traits<_Ty>::iterator_category>;

    template<class _Ty>
    using bidirectional_iterator_tag_cond = typename MPL_L::enable_if_t<::std::is_base_of_v<bidirectional_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>, typename iterator_traits<_Ty>::iterator_category>;

    template<class _Ty>
    using random_access_iterator_tag_cond = typename MPL_L::enable_if_t<::std::is_base_of_v<random_access_iterator_tag,
        typename iterator_traits<_Ty>::iterator_category>, typename iterator_traits<_Ty>::iterator_category>;

    #endif


    template<class _Iterty>
    using difference_type_t = typename iterator_traits<_Iterty>::difference_type;


    template<output_iterator_tag_cond _Ty, class _IterDistance>
    IOP_CONSTEXPR_CXX17 void __advance(_Ty& __i, _IterDistance __n) NOEXCEPT {}

    template<input_iterator_tag_cond _IterStart, input_iterator_tag_cond _IterDistance>
    IOP_CONSTEXPR_CXX17 void __advance(_IterStart& __i, _IterDistance __n)NOEXCEPT {
        while (--__n) ++__i;
    }

    template<bidirectional_iterator_tag_cond _Ty, class _IterDistance>
    IOP_CONSTEXPR_CXX17 void __advance(_Ty& __i, _IterDistance __n) NOEXCEPT {
        if (__n >= 0)
            while (__n--) ++__i;
        else
            while (__n++) --__i;
    }

    template<random_access_iterator_tag_cond _Ty, class _IterDistance>
    IOP_CONSTEXPR_CXX17 void __advance(_Ty& __i, _IterDistance __n) NOEXCEPT {
        __i += __n;
    }

    template<class _Ty, class _D>
    IOP_CONSTEXPR_CXX17 void advance(_Ty& __i, _D __n) NOEXCEPT { __advance(__i, __n); }


    template <input_iterator_tag_cond _Ty>
    IOP_CONSTEXPR_CXX17 difference_type_t<_Ty>
        __distance(_Ty __frist, _Ty __last) NOEXCEPT
    {
        difference_type_t<_Ty> __n = 0;
        while (__frist != __last) {
            ++__frist; ++__n;
        }
        return __n;
    }

    template<random_access_iterator_tag_cond _Ty>
    IOP_CONSTEXPR_CXX17 difference_type_t<_Ty>
        __distance(_Ty __frist, _Ty __last) NOEXCEPT {
        return static_cast<difference_type_t<_Ty>>(__frist - __last);
    }

    template<class _Ty>
    IOP_CONSTEXPR_CXX17 difference_type_t<_Ty>
        distance(_Ty __frist, _Ty __last) NOEXCEPT {
        return __distance(__frist, __last);
    }


    template<bidirectional_iterator_tag_cond _Ty>
    IOP_CONSTEXPR_CXX17 _Ty __copy_backward(_Ty __frist, _Ty __last, _Ty __res)
        NOEXCEPT{
            if (__frist != __last)
                *(--__res) = *(--__last);
            return __res;
    }

        template<random_access_iterator_tag_cond _Ty, class _Distance>
    IOP_CONSTEXPR_CXX17 _Ty __copy_backward(_Ty __frist, _Ty __last, _Ty __res)
        NOEXCEPT{
            for (_Distance __n = __last - __frist;__n > 0;--__n)
                *(--__res) = *(--__last);
            return __res;
    }

        template<class _Ty>
    IOP_CONSTEXPR_CXX17 _Ty copy_backward(_Ty __frist, _Ty __last, _Ty __res) NOEXCEPT {
        return __copy_backward(__frist, __last, __res);
    }


    template <class _Objty, class _Ty>
    IOP_CONSTEXPR_CXX17 void construct(_Objty* __ptr, const _Ty& __v)
        NOEXCEPT(std::is_nothrow_constructible_v<_Objty>) {
        ::new (_Voidify_ptr(__ptr)) _Objty(__v);
    }

    template <class _Objty>
    IOP_CONSTEXPR_CXX17 void construct(_Objty* __ptr) {
        construct(__ptr, _Objty());
    }

    template <class _Objty, class... _Args>
    IOP_CONSTEXPR_CXX17 void construct(_Objty* __ptr, _Args... __args)
        NOEXCEPT(std::is_nothrow_constructible_v<_Objty>) {
        ::new (_Voidify_ptr(__ptr)) _Objty{ FVstd::forward<_Args>(__args)... };
    }

    template<class _Ty>
    IOP_CONSTEXPR_CXX17 void _destroy(_Ty* const __res) { __res->~_Ty(); }

    template<input_iterator_tag_cond _Iter>
    IOP_CONSTEXPR_CXX17 void destroy(_Iter __frist, _Iter __last) {
        for (;__frist != __last;++__frist)
            _destroy(&*__frist);
    }

    template<class _Iter>
    IOP_CONSTEXPR_CXX17 void destroy(_Iter __res) { _destroy(&*__res); }


    template<forward_iterator_tag_cond _Ty, class _Size, class _Tv>
    IOP_CONSTEXPR_CXX17 void uninitialized_fill_n(_Ty __frist, _Size __n, _Tv __val) {
        fill_n(__frist, __n, __val);
    }

    template<class _Ty, class _Tv>
    IOP_CONSTEXPR_CXX17 void uninitialized_fill(_Ty __frist, _Tv __val) {
        fill(__frist, __val);
    }


    template<input_iterator_tag_cond _Ty, class _Size, forward_iterator_tag_cond _Tp>
    ::std::pair<_Ty, _Tp> uninitialized_copy_n(_Ty __frist, _Size __n, _Tp __iter_data) {
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

    template<input_iterator_tag_cond _Ty, forward_iterator_tag_cond _Tp>
    IOP_CONSTEXPR_CXX17 _Tp uninitialized_copy(_Ty __frist, _Ty __last, _Tp __iter_data) {
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


    template<class _Ty>
    IOP_CONSTEXPR_CXX17 auto copy(_Ty __q, _Ty __p, _Ty __res) {
        return ::std::copy(__q, __p, __res);
    }


    template<class _Ty>
    IOP_CONSTEXPR_CXX17 _Ty find(_Ty __first, _Ty __last, ::std::size_t __v) {
        _Ty it = __first;
        for (;it != __last;++it) {
            if (*it == __v)
                break;
        }
        return it;
    }

    template<class _Ty>
    IOP_CONSTEXPR_CXX17 void swap(_Ty __q, _Ty __p) {
        auto v = *__q;
        *__q = *__p;
        *__p = v;
    }


    template<class _Contaier, ::std::size_t _N>
    IOP_CONSTEXPR_CXX17 typename _Contaier::value_type&& get(_Contaier&& __src) NOEXCEPT {
        return
            FVstd::forward<typename _Contaier::value_type>(FVstd::forward<_Contaier>(__src)[_N]);
    }


    template<class _Contaier, class _Ty>
    void resize(_Contaier& __obj, size_t __n, const _Ty& __v)
        NOEXCEPT(NOEXCEPT(assert(__n >= __obj.max_size()))) {
        if (__n > __obj.size())
            for (;__n != 0;--__n) __obj.push_back(__v);
        else if (__n != __obj.size())
            for (;__n != 0; --__n) __obj.pop_back();
    }

    template<class _Contaier, class _Ty>
    void resize(_Contaier& __obj, size_t __n) { resize(__obj, __n, _Ty()); }

};      // namespace IOP_STL

#endif          //*  _ITERATOR_BASE_IOP_

