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
#include "utilitys.h"
#include "type_traits.h"
#include "metacomponent.h"
#include "cppconfig.h"


#include <new>      
#include <utility>
#include <assert.h>
#include <algorithm>

#define ITERAOTR_TAG_DEFINE(NAME,IS_A_OBJ)                          \
    template<class _Ty, class _Distance>                            \
    struct NAME : public IS_A_OBJ<_Ty, _Distance> {                 \
        using iterator_category  = NAME##_tag;                      \
        using value_type = _Ty;                                     \
        using pointer = _Ty*;                                       \
        using reference = _Ty&;                                     \
        using difference_type = _Distance;                          \
    };


#define IS_ITERATOR_TAG_TOTAL(ITERATOR_TAG)                                \
    template<class _Ty>                                                    \
    using is_##ITERATOR_TAG = mpls::is_same<_Ty, ITERATOR_TAG>;            \
    template<class _Ty>                                                    \
    using is_##ITERATOR_TAG##_t = typename is_##ITERATOR_TAG<_Ty>::type;   \
    template<class _Ty>                                                    \
    constexpr bool is_##ITERATOR_TAG##_v = is_##ITERATOR_TAG<_Ty>::value;  


namespace iop {

    // type 
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    // iterator
    template<class _Category,
        class _Ty,
        class _Distance = ::std::ptrdiff_t,
        class _Pointer = _Ty*,
        class _Reference = _Ty&>
    class iterator {
    public:
        using iterator_category = _Category;

        using value_type = _Ty;
        using pointer = _Ty*;
        using reference = _Ty&;
        using difference_type = _Distance;
    };


    template<class _Ty, class _Distance>
    struct input_iterator {
        using iterator_category = input_iterator_tag;

        using value_type = _Ty;
        using pointer = _Ty*;
        using reference = _Ty&;
        using difference_type = _Distance;
    };


    struct output_iterator {
        using iterator_category = output_iterator_tag;

        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
    };


    ITERAOTR_TAG_DEFINE(forward_iterator, input_iterator);
    ITERAOTR_TAG_DEFINE(bidirectional_iterator, forward_iterator);
    ITERAOTR_TAG_DEFINE(random_access_iterator, bidirectional_iterator);


    // iterator traits
    template <class _Iterator>
    struct iterator_traits {
        using iterator_category = typename _Iterator::iterator_category;

        using value_type = typename _Iterator::value_type;
        using pointer = typename _Iterator::pointer;
        using reference = typename _Iterator::reference;
        using difference_type = typename _Iterator::difference_type;
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


    template <class I>
    inline typename iterator_traits<I>::iterator_category
        iterator_category(const I&) {
        using category = typename iterator_traits<I>::iterator_category;
        return category();
    }

    template<class _Iterty>
    using difference_type_t = typename iterator_traits<_Iterty>::difference_type;


    // type_traits utli
    IS_ITERATOR_TAG_TOTAL(input_iterator_tag);
    IS_ITERATOR_TAG_TOTAL(output_iterator_tag);
    IS_ITERATOR_TAG_TOTAL(forward_iterator_tag);
    IS_ITERATOR_TAG_TOTAL(bidirectional_iterator_tag);
    IS_ITERATOR_TAG_TOTAL(random_access_iterator_tag);



    template <class _InputIterator, class _Distance>
    inline void __distance(_InputIterator __first, _InputIterator __last,
        _Distance& __n, input_iterator_tag)
    {
        while (__first != __last) { ++__first; ++__n; }
    }

    template <class _RandomAccessIterator, class _Distance>
    inline void __distance(_RandomAccessIterator __first,
        _RandomAccessIterator __last,
        _Distance& __n, random_access_iterator_tag)
    {
        __n += __last - __first;
    }

    template <class _Iterator, class _Distance>
    inline void distance(_Iterator __first,
        _Iterator __last, _Distance& __n) {
        __distance(__first, __last, __n, iterator_category(__first));
    }

    template <class _Iterator>
    auto distance(_Iterator __first, _Iterator __last) {
        difference_type_t<_Iterator> n = 0;
        __distance(__first, __last, n, iterator_category(__first));
        return n;
    }

    template<class _Ty, class _IterDistance>
    void __advance(_Ty& __i, _IterDistance __n, input_iterator_tag) NOEXCEPT {}

    template<class _IterStart, class _IterDistance>
    void __advance(_IterStart& __i, _IterDistance __n, forward_iterator_tag) NOEXCEPT
    {
        while (--__n) ++__i;
    }

    template<class _Ty, class _IterDistance>
    void __advance(_Ty& __i, _IterDistance __n, bidirectional_iterator_tag) NOEXCEPT
    {
        if (__n >= 0)
            while (__n--) ++__i;
        else
            while (__n++) --__i;
    }

    template<class _Ty, class _IterDistance>
    void __advance(_Ty& __i, _IterDistance __n, random_access_iterator_tag) NOEXCEPT
    {
        __i += __n;
    }

    template<class _Ty, class _D>
    void advance(_Ty& __i, _D __n) NOEXCEPT { __advance(__i, __n, iterator_category(__i)); }



    template<class _Ty>
    _Ty __copy_backward(_Ty __frist, _Ty __last, _Ty __res, bidirectional_iterator_tag) NOEXCEPT {
        while (__last != __frist)
            *(--__res) = *(--__last);
        return __res;
    }

    template<class _Ty, class _Distance>
    _Ty __copy_backward(_Ty __frist, _Ty __last, _Ty __res, random_access_iterator_tag) NOEXCEPT {
        for (_Distance __n = __last - __frist;__n > 0;--__n)
            *(--__res) = *(--__last);
        return __res;
    }


    template<class _Ty>
    _Ty copy_backward(_Ty __frist, _Ty __last, _Ty __res) NOEXCEPT {
        return __copy_backward(__frist, __last, __res, iterator_category(__frist));
    }


    template <class _Ty, class _Tv>
    constexpr _Ty* construct_at(_Ty* __ptr, const _Tv& __v)
        NOEXCEPT(::std::is_nothrow_constructible_v<_Ty>) {
        return ::new (static_cast<void*>(__ptr)) _Ty(__v);
    }

    template <class _Ty>
    constexpr _Ty* construct_at(_Ty* __ptr) {
        return construct_at(__ptr, _Ty());
    }

    template <class _Ty, class... _Args>
    constexpr _Ty* construct_at(_Ty* __ptr, _Args... __args)
        NOEXCEPT(::std::is_nothrow_constructible_v<_Ty>) {
        return ::new ((__ptr)) _Ty{ FViop::forward<_Args>(__args)... };
    }

    template<class _Ty>
    constexpr void destroy_at(_Ty* __res) {
        if constexpr (mpls::is_array_v<_Ty>) {
            for (auto& elem : *__res)
                destroy_at(&elem);
        }
        else
            __res->~_Ty();
    }

    template<class Iter>
    constexpr void destroy(Iter __frist, Iter __last) {
        for (;__frist != __last;++__frist)
        //  error iop::destroy_at
            ::std::destroy_at(::std::addressof(*__frist));
    }

    template<class Iter>
    constexpr void destroy(Iter __res) {
        destroy_at(::std::addressof(*__res));
    }

    template <class Iter, class _Tv>
    constexpr void construct(Iter __ptr, const _Tv& __v) {
        construct_at(::std::addressof(*__ptr), __v);
    }

    template <class Iter>
    constexpr void construct(Iter* __ptr) {
        construct(__ptr, Iter());
    }


    template<class _Ty, class _Size, class _Tv>
    void uninitialized_fill_n(_Ty __frist, _Size __n, _Tv __val) {
        fill_n(__frist, __n, __val);
    }

    template<class _Ty, class _Tv>
    void uninitialized_fill(_Ty __frist, _Tv __val) {
        fill(__frist, __val);
    }


    template<class InputIt, class NoThrowForwardIt>
    NoThrowForwardIt uninitialized_copy(InputIt first, InputIt last, NoThrowForwardIt d_first)
    {
        using T = typename iterator_traits<NoThrowForwardIt>::value_type;
        NoThrowForwardIt current = d_first;
        try
        {
            for (; first != last; ++first, (void) ++current)
                ::new (static_cast<void*>(::std::addressof(*current))) T(*first);
            return current;
        }
        catch (...)
        {
            for (; d_first != current; ++d_first)
                d_first->~T();
            throw;
        }
    }

    template<class _Ty, class _Size, class _Tp>
    auto uninitialized_copy_n(_Ty first, _Size __n, _Tp d_first) {
        assert(__n > 0);
        using T = typename iterator_traits<_Tp>::value_type;
        _Ty current = d_first;
        try
        {
            for (; __n > 0; --__n, ++first, (void) ++current)
                ::new (static_cast<void*>(::std::addressof(*current))) T(*first);
            return current;
        }
        catch (...)
        {
            for (; d_first != current; ++d_first)
                d_first->~T();
            throw;
        }
    }

};      // namespace iop



#endif          //*  _ITERATOR_BASE_IOP_

